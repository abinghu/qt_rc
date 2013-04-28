// RsDecode.cpp: implementation of the RsDecode class.
//
//////////////////////////////////////////////////////////////////////

#include "RsDecode.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/*
数组长度
sg0===npar
sg1===npar
sigma===npar/2+2
omega===npar/2+1
syn===npar
*/

RsDecode::RsDecode()
{
	galois = Galois();
}

RsDecode::~RsDecode()
{

}

RsDecode::RsDecode(int npar)
{
	galois = Galois();
	this->npar = npar;
}

/**
 * Modified Berlekamp-Massey
 *
 * @param sigma int[]
 *		σ(z)格納用配列、最大npar/2 + 2個の領域が必要
 * 		σ0,σ1,σ2, ... σ<jisu>
 * @param omega int[]
 *		ω(z)格納用配列、最大npar/2 + 1個の領域が必要
 *		ω0,ω1,ω2, ... ω<jisu-1>
 * @param syn int[]
 *		シンドローム配列
 * 		s0,s1,s2, ... s<npar-1>
 * @return int
 * 		>= 0: σの次数
 * 		< 0: エラー
 */
int RsDecode::calcSigmaMBM(int * sigma, int * omega, int * syn)
{
	int * sg0 = new int[npar];
	int * sg1 = new int[npar];
	for(int i=0;i<npar;i++)
	{
		sg0[i]=0;sg1[i]=0;
	}
	sg0[1] = 1;
	sg1[0] = 1;
	int jisu0 = 1;
	int jisu1 = 0;
	int m = -1;

	for(int n = 0; n < npar; n++) {
		// 判別式を計算
		int d = syn[n];
		for(int i = 1; i <= jisu1; i++) {
			d ^= galois.mul(sg1[i], syn[n - i]);
		}
		if(d != 0) {
			int logd = galois.toLog(d);
			int * wk = new int[npar];
			for(int i=0;i<npar;i++)
				wk[i]=0;
			for(int i = 0; i <= n; i++) {
				wk[i] = sg1[i] ^ galois.mulExp(sg0[i], logd);
			}
			int js = n - m;
			if(js > jisu1) {
				m = n - jisu1;
				jisu1 = js;
				if(jisu1 > npar / 2) {
					return -1;				// σの次数がnpar / 2を超えたらエラー
				}
				for(int i = 0; i <= jisu0; i++) {
					sg0[i] = galois.divExp(sg1[i], logd);
				}
				jisu0 = jisu1;
			}
			sg1 = wk;
		}

		int cnt=npar-1<jisu0 ? npar-1 : jisu0;
		for(int i=cnt-1;i>=0;i--)
			sg0[i+1]=sg0[i];

		sg0[0] = 0;
		jisu0++;
	}
	galois.mulPoly(omega, sg1, syn, npar/2+1, npar, npar);

	int cnt=npar<npar/2+2 ? npar : npar/2+2;
	for(int i=0;i<cnt;i++)
		sigma[i]=sg1[i];

	return jisu1;
}

/**
 * チェン探索により誤り位置を求める
 *		σ(z) = 0の解を探索する
 *		ただし、探索はデータ長以内の解のみで
 *		jisu個の解が見つからなければ、エラーとする
 * @param pos int[]
 * 		誤り位置格納用配列、jisu個の領域が必要
 * @param n int
 * 		データ長
 * @param jisu int
 * 		σの次数
 * @param sigma int[]
 * 		σ0,σ1,σ2, ... σ<jisu>
 * @return int
 *		0: 正常終了
 *		< 0: エラー
 */
int RsDecode::chienSearch(int * pos, int n, int jisu, int * sigma)
{
	/*
	 * σ(z) = (1-α^i*z)(1-α^j*z)(1-α^k*z)
	 *       = 1 + σ1z + σ2z^2 +...
	 * σ1 = α^i + α^j + α^k
	 * 上記の性質を利用して、プチ最適化
	 * last = σ1から、見つけた解を次々と引いていくことにより、最後の解はlastとなる
	 */
	int last = sigma[1];

	if(jisu == 1) {
		// 次数が1ならば、lastがその解である
		if(galois.toLog(last) >= n) {
			return RS_CORRECT_ERROR;	// 範囲外なのでエラー
		}
		pos[0] = last;
		return 0;
	}

	int posIdx = jisu - 1;		// 誤り位置格納用インデックス
	for(int i = 0; i < n; i++) {
		/*
		 * σ(z)の計算
		 * w を1(0乗の項)に初期化した後、残りの項<1..jisu>を加算
		 * z = 1/α^i = α^Iとすると
		 * σ(z) = 1 + σ1α^I + σ2(α^I)^2 + σ3(α^I)^3 + ... + σ<jisu>/(α^I)^<jisu>
		 *       = 1 + σ1α^I + σ2α^(I*2) + σ3α^(I*3) + ... + σ<jisu>α^(I*<jisu>)
		 */
		int z = 255 - i;					// z = 1/α^iのスカラー
		int wk = 1;
		for(int j = 1; j <= jisu; j++) {
			wk ^= galois.mulExp(sigma[j], (z * j) % 255);
		}
		if(wk == 0) {
			int pv = galois.toExp(i);		// σ(z) = 0の解
			last ^=  pv;					// lastから今見つかった解を引く
			pos[posIdx--] = pv;
			if(posIdx == 0) {
				// 残りが一つならば、lastがその解である
				if(galois.toLog(last) >= n) {
					return RS_CORRECT_ERROR;	// 最後の解が範囲外なのでエラー
				}
				pos[0] = last;
				return 0;
			}
		}
	}
	// 探索によりデータ長以内に、jisu個の解が見つからなかった
	return RS_CORRECT_ERROR;
}

/**
 * Forney法で誤り訂正を行う
 *		σ(z) = (1-α^i*z)(1-α^j*z)(1-α^k*z)
 *		σ'(z) = α^i * (1-α^j*z)(1-α^k*z)...
 *			   + α^j * (1-α^i*z)(1-α^k*z)...
 *			   + α^k * (1-α^i*z)(1-α^j*z)...
 *		ω(z) = (E^i/(1-α^i*z) + E^j/(1-α^j*z) + ...) * σ(z)
 *			  = E^i*(1-α^j*z)(1-α^k*z)...
 *			  + E^j*(1-α^i*z)(1-α^k*z)...
 *			  + E^k*(1-α^i*z)(1-α^j*z)...
 *		∴ E^i = α^i * ω(z) / σ'(z)
 * @param data int[]
 *		入力データ配列
 * @param length int
 *		入力データ長さ
 * @param jisu int
 *		σの次数
 * @param pos int[]
 *		誤り位置配列
 * @param sigma int[]
 *		σ0,σ1,σ2, ... σ<jisu>
 * @param omega int[]
 *		ω0,ω1,ω2, ... ω<jisu-1>
 */
void RsDecode::doForney(unsigned char * data, int length, int jisu, int * pos, int * sigma, int * omega)
{
	int i,j;
	for(i = 0; i < jisu; i++)
	{
		int ps = pos[i];
		int zlog = 255 - galois.toLog(ps);					// zのスカラー

		// ω(z)の計算
		int ov = omega[0];
		for(j = 1; j < jisu; j++)
		{
			ov ^= galois.mulExp(omega[j], (zlog * j) % 255);		// ov += ωi * z^j
		}

		// σ'(z)の値を計算(σ(z)の形式的微分)
		int dv = sigma[1];
		for(j = 2; j < jisu; j += 2)
		{
			dv ^= galois.mulExp(sigma[j + 1], (zlog * j) % 255);	// dv += σ<j+1> * z^j
		}

		/*
		 * 誤り訂正 E^i = α^i * ω(z) / σ'(z)
		 * 誤り位置の範囲はチェン探索のときに保証されているので、
		 * ここではチェックしない
		 */
		data[galois.toPos(length, ps)] ^= galois.mul(ps, galois.div(ov, dv));
	}
}

/**
 * RSコードのデコード
 *
 * @param data int[]
 *		入力データ配列
 * @param length int
 * 		パリティを含めたデータ長
 * @param noCorrect boolean
 * 		チェックのみで訂正は行わない
 * @return int
 * 		0: エラーなし
 * 		> 0: 戻り値個の誤りを訂正した
 * 		< 0: 訂正不能
 */
int RsDecode::decode(unsigned char * data, int length, bool noCorrect)
{
	if(length < npar || length > 255) {
		return RS_PERM_ERROR;
	}
	// シンドロームを計算
	int * syn = new int[npar];
	for(int i=0;i<npar;i++)syn[i]=0;

	if(galois.calcSyndrome(data, length, syn, npar)) {
		return 0;		// エラー無し
	}
	// シンドロームよりσとωを求める
	int * sigma = new int[npar/2+2];
	for(int i=0;i<npar/2+2;i++)sigma[i]=0;
	int * omega = new int[npar/2+1];
	for(int i=0;i<npar/2+1;i++)omega[i]=0;
	int jisu = calcSigmaMBM(sigma, omega, syn);
	if(jisu <= 0) {
		return RS_CORRECT_ERROR;
	}
	// チェン探索により誤り位置を求める
	int * pos = new int[jisu];
	for(int i=0;i<jisu;i++)pos[i]=0;

	int r = chienSearch(pos, length, jisu, sigma);
	if(r < 0) {
		return r;
	}
	if(!noCorrect) {
		// 誤り訂正
		doForney(data, length, jisu, pos, sigma, omega);
	}
	return jisu;
}

int RsDecode::decode(unsigned char * data, int length)
{
	return decode(data, length, false);
}

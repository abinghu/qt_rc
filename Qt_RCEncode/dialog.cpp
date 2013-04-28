#include "dialog.h"
#include "ui_dialog.h"

#include <QFile>
#include <QTextStream>
#include <QFileDialog>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog),m_bDataEncoded(false)
{
    ui->setupUi(this);

    // 初始化控件
    QStringList level;
    level << "L(7%)" << "M(15%)" << "Q(25%)" << "H(30%)";
    ui->level_combobox->addItems(level);
    ui->level_combobox->setCurrentIndex(1);

    QStringList version;
    version << tr("默认");
    for(int index=1; index<=40; ++index)
        version << QString::number(index);
    ui->version_combobox->addItems(version);

    QStringList mask;
    mask << tr("默认");
    for(int index=0; index<=7; ++index)
        mask << QString::number(index);
    ui->mask_combobox->addItems(mask);

    ui->spinBox->setValue(4);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_textEdit_textChanged()
{
	// 获取二维码生成条件
    int level_index = ui->level_combobox->currentIndex();
    int version_index = ui->version_combobox->currentIndex();
    bool bExtent = (ui->checkBox->checkState() == Qt::Checked);
    int mask_index = ui->mask_combobox->currentIndex()-1;
    QString encode_string = ui->textEdit->toPlainText();

    m_bDataEncoded = m_QREncode.EncodeData(level_index, version_index, 
		bExtent, mask_index, encode_string.toLocal8Bit().data());

	if (m_bDataEncoded == false)
	{
		ui->image_label->clear();
		ui->image_label->setText(tr("请输入二维码信息..."));
		ui->size_label->clear();
		return;
	}

    // 创建二维码图片
	m_EncodeImageSize = m_QREncode.m_nSymbleSize + (QR_MARGIN * 2);
	QImage encode_image(m_EncodeImageSize, m_EncodeImageSize, QImage::Format_Mono);
	encode_image.fill(1);

    // 生成二维码图片
	for (int i=0; i<m_QREncode.m_nSymbleSize; ++i)
	{
		for (int j=0; j<m_QREncode.m_nSymbleSize; ++j)
		{
			if (m_QREncode.m_byModuleData[i][j])
				encode_image.setPixel(i+QR_MARGIN, j+QR_MARGIN, 0);
		}
	}
	m_EncodeImage = encode_image;

	// 显示二维码规格大小
	on_spinBox_valueChanged(0);

    // 显示二维码图片
	QImage scale_image = encode_image.scaled(ui->image_label->width(), ui->image_label->height());
    ui->image_label->setPixmap(QPixmap::fromImage(scale_image));
}

void Dialog::on_level_combobox_currentIndexChanged(int index)
{
	on_textEdit_textChanged();
}

void Dialog::on_version_combobox_currentIndexChanged(int index)
{
	on_textEdit_textChanged();
}

void Dialog::on_mask_combobox_currentIndexChanged(int index)
{
	on_textEdit_textChanged();
}

void Dialog::on_checkBox_clicked()
{
	on_textEdit_textChanged();
}

void Dialog::on_spinBox_valueChanged(int arg1)
{
	if (m_bDataEncoded)
	{
		int scale = ui->spinBox->value();
		int scale_size = m_EncodeImageSize*scale;

		QString size_info;
		QTextStream(&size_info) << scale_size << " * " << scale_size;
		ui->size_label->setText(size_info);
	}
}

void Dialog::on_save_button_clicked()
{
	if (m_bDataEncoded)
	{
		// 打开保存对话框
		QString file_name = QFileDialog::getSaveFileName(this, tr("save file"), tr("RC.png"), 
			tr("Image file (*.png)"));

		if (!file_name.isNull())
		{
			int scale_size = m_EncodeImageSize*ui->spinBox->value();
			QImage scale_image = m_EncodeImage.scaled(scale_size, scale_size);
			scale_image.save(file_name);
		}
	}
}

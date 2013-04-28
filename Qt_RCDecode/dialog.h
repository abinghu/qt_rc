#ifndef DIALOG_H
#define DIALOG_H

#include "QRcodeImage.h"

#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    
private slots:
    void on_pushButton_clicked();

private:
    Ui::Dialog *ui;

    QImage m_EncodeImage;
	QRcodeImage *m_pDecodeImage;
	unsigned char m_CodeData[MAX_MODULESIZE][MAX_MODULESIZE]; // 存储二维码位数据
};

#endif // DIALOG_H

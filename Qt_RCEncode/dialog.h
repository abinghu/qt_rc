#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "QR_Encode.h"

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
    void on_textEdit_textChanged();

    void on_level_combobox_currentIndexChanged(int index);

    void on_version_combobox_currentIndexChanged(int index);

    void on_mask_combobox_currentIndexChanged(int index);

    void on_checkBox_clicked();

    void on_spinBox_valueChanged(int arg1);

    void on_save_button_clicked();

private:
    Ui::Dialog *ui;

	// ¶þÎ¬Âë±àÂëºËÐÄÀà
	CQR_Encode  m_QREncode;
	QImage		m_EncodeImage;
	bool		m_bDataEncoded;
	int			m_EncodeImageSize;
};

#endif // DIALOG_H

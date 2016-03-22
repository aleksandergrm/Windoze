#include <QMessageBox>
#include "filetype.h"
#include "ui_filetype.h"

FileType::FileType(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileType)
{
    ui->setupUi(this);
    mType = Null;
}

FileType::~FileType()
{
    delete ui;
}

void FileType::on_OKButton_clicked()
{
    if(ui->rbBinary->isChecked()) {
        mType= Binary;
    }
    else if(ui->rbAscii->isChecked()) {
        mType = Ascii;
    } else {
        QMessageBox::warning(this, tr("Warning"), "Data type must me set!");
        return;
    }

    this->close();
}

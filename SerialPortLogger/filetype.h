#ifndef FILETYPE_H
#define FILETYPE_H

#include <QDialog>

namespace Ui {
class FileType;
}

class FileType : public QDialog
{
    Q_OBJECT
public:
    explicit FileType(QWidget *parent = 0);
    ~FileType();

    enum Type { Null, Binary, Ascii };

    void setType(Type t) { mType = t; }
    Type getType() { return mType; }

private slots:
    void on_OKButton_clicked();

private:
    Ui::FileType *ui;
    Type mType;
};

#endif // FILETYPE_H

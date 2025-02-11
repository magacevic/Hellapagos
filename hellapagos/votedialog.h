#ifndef VOTEDIALOG_H
#define VOTEDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>

class VoteDialog : public QDialog
{
    Q_OBJECT

public:
    explicit VoteDialog(const QStringList& playerNames, QWidget *parent = nullptr);
    ~VoteDialog();

    QString getSelectedPlayerName() const;
    void onConfirmClicked();

private:
    QComboBox *comboBox;
    QStringList playerNames;
};

#endif // VOTEDIALOG_H

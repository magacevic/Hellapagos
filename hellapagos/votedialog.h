#ifndef VOTEDIALOG_H
#define VOTEDIALOG_H

#include <QDialog>
#include <QStringList>

namespace Ui {
class VoteDialog;
}

class VoteDialog : public QDialog
{
    Q_OBJECT

public:
    explicit VoteDialog(const QStringList &playerNames, QWidget *parent = nullptr);
    ~VoteDialog();

    QString getSelectedPlayerName() const;

private slots:
    void onVoteButtonClicked();

private:
    Ui::VoteDialog *ui;
    QString selectedPlayer;
};

#endif // VOTEDIALOG_H

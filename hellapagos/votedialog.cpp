#include "votedialog.h"
#include "./ui_votedialog.h"

VoteDialog::VoteDialog(const QStringList& players, QWidget* parent)
    : QDialog(parent), ui(new Ui::VoteDialog), selectedPlayer("") {
    ui->setupUi(this);
    ui->comboBoxPlayers->addItems(players);

    connect(ui->voteButton, &QPushButton::clicked, this, &VoteDialog::onVoteButtonClicked);
}

VoteDialog::~VoteDialog() {
    delete ui;
}

void VoteDialog::onVoteButtonClicked() {
    selectedPlayer = ui->comboBoxPlayers->currentText();
    accept(); // closes dialog
}

QString VoteDialog::getSelectedPlayerName() const {
    return selectedPlayer;
}

#include "votedialog.h"
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>

VoteDialog::VoteDialog(const QStringList& playerNames, QWidget * parent)
    : QDialog(parent), playerNames(playerNames)
{
    setWindowTitle("Vote for a player");

    comboBox = new QComboBox(this);
    for (const QString& name : playerNames) {
        comboBox->addItem(name);
    }

    // Create the Confirm button
    QPushButton* confirmButton = new QPushButton("Confirm", this);
    connect(confirmButton, &QPushButton::clicked, this, &VoteDialog::onConfirmClicked);

    // Layout
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(comboBox);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(confirmButton);

    layout->addLayout(buttonLayout);
    setLayout(layout);
}

VoteDialog::~VoteDialog() {}

QString VoteDialog::getSelectedPlayerName() const
{
    return comboBox->currentText();  // Return the selected player name
}

void VoteDialog::onConfirmClicked() {
    // Check if a player is selected, then accept the dialog
    if (!comboBox->currentText().isEmpty()) {
        accept();
    } else {
        QMessageBox::warning(this, "No selection", "You must select a player to vote for.");
    }
}


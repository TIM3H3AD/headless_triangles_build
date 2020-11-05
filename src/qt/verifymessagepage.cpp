#include "verifymessagepage.h"
#include "ui_verifymessagepage.h"

#include "addressbookpage.h"
#include "base58.h"
#include "guiutil.h"
#include "init.h"
#include "main.h"
#include "optionsmodel.h"
#include "walletmodel.h"
#include "wallet.h"

#include <string>
#include <vector>

#include <QClipboard>
#include <QDesktopWidget>

VerifyMessagePage::VerifyMessagePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VerifyMessagePage),
    model(0)
{
    ui->setupUi(this);

#if (QT_VERSION >= 0x040700)
    /* Do not move this to the XML file, Qt before 4.7 will choke on it */
    ui->addressIn_VM->setPlaceholderText(tr("Enter a Triangles address (e.g. TXc7mPCNFFpinDonuSH5PNVY9S8nBcvGQm)"));
    ui->signatureIn_VM->setPlaceholderText(tr("Enter Triangles signature"));
#endif

    GUIUtil::setupAddressWidget(ui->addressIn_VM, this);

    ui->addressIn_VM->installEventFilter(this);
    ui->messageIn_VM->installEventFilter(this);
    ui->signatureIn_VM->installEventFilter(this);

    ui->signatureIn_VM->setFont(GUIUtil::trianglesAddressFont());

    // To make size minimal
    ui->statusLabel_VM->setText(tr("The entered address is invalid.") + QString(" ") + tr("Please check the address and try again."));
    ui->statusLabel_VM->clear();
}

VerifyMessagePage::~VerifyMessagePage()
{
    delete ui;
}

void VerifyMessagePage::setModel(WalletModel *model)
{
    this->model = model;
}

void VerifyMessagePage::setAddress_VM(const QString &address)
{
    ui->addressIn_VM->setText(address);
    ui->messageIn_VM->setFocus();
}

void VerifyMessagePage::showTab_VM(bool fShow)
{
    // Reserved
}

void VerifyMessagePage::on_addressBookButton_VM_clicked()
{
    if (model && model->getAddressTableModel())
    {
        AddressBookPage dlg(AddressBookPage::ForSending, AddressBookPage::SendingTab, this);
        dlg.setModel(model->getAddressTableModel());
        if (dlg.exec())
        {
            setAddress_VM(dlg.getReturnValue());
        }
    }
}

void VerifyMessagePage::on_verifyMessageButton_VM_clicked()
{
    CTrianglesAddress addr(ui->addressIn_VM->text().toStdString());
    if (!addr.IsValid())
    {
        ui->addressIn_VM->setValid(false);
        ui->statusLabel_VM->setStyleSheet("QLabel { color: red; }");
        ui->statusLabel_VM->setText(tr("The entered address is invalid.") + QString(" ") + tr("Please check the address and try again."));
        return;
    }
    CKeyID keyID;
    if (!addr.GetKeyID(keyID))
    {
        ui->addressIn_VM->setValid(false);
        ui->statusLabel_VM->setStyleSheet("QLabel { color: red; }");
        ui->statusLabel_VM->setText(tr("The entered address does not refer to a key.") + QString(" ") + tr("Please check the address and try again."));
        return;
    }

    bool fInvalid = false;
    std::vector<unsigned char> vchSig = DecodeBase64(ui->signatureIn_VM->text().toStdString().c_str(), &fInvalid);

    if (fInvalid)
    {
        ui->signatureIn_VM->setValid(false);
        ui->statusLabel_VM->setStyleSheet("QLabel { color: red; }");
        ui->statusLabel_VM->setText(tr("The signature could not be decoded.") + QString(" ") + tr("Please check the signature and try again."));
        return;
    }

    CDataStream ss(SER_GETHASH, 0);
    ss << strMessageMagic;
    ss << ui->messageIn_VM->document()->toPlainText().toStdString();

    CKey key;
    if (!key.SetCompactSignature(Hash(ss.begin(), ss.end()), vchSig))
    {
        ui->signatureIn_VM->setValid(false);
        ui->statusLabel_VM->setStyleSheet("QLabel { color: red; }");
        ui->statusLabel_VM->setText(tr("The signature did not match the message digest.") + QString(" ") + tr("Please check the signature and try again."));
        return;
    }

    if (!(CTrianglesAddress(key.GetPubKey().GetID()) == addr))
    {
        ui->statusLabel_VM->setStyleSheet("QLabel { color: red; }");
        ui->statusLabel_VM->setText(QString("<nobr>") + tr("Message verification failed.") + QString("</nobr>"));
        return;
    }

    ui->statusLabel_VM->setStyleSheet("QLabel { color: #AF272E; }");
    ui->statusLabel_VM->setText(QString("<nobr>") + tr("Message verified.") + QString("</nobr>"));
}

void VerifyMessagePage::on_clearButton_VM_clicked()
{
    ui->addressIn_VM->clear();
    ui->signatureIn_VM->clear();
    ui->messageIn_VM->clear();
    ui->statusLabel_VM->clear();

    ui->addressIn_VM->setFocus();
}

bool VerifyMessagePage::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress || event->type() == QEvent::FocusIn)
    {
        /* Clear status message on focus change */
        ui->statusLabel_VM->clear();
    }
    return QWidget::eventFilter(object, event);
}

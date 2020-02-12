#include "settings_dlg.h"
#include "i18n_helper.h"
#include "settings_mgr.h"
#include "mainwindow.h"
#include "reader_applet.h"
#include "auto_update_service.h"

SettingsDlg::SettingsDlg(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    mLangCombo->addItems(I18NHelper::getInstance()->getLanguages());

    initialize();
}

SettingsDlg::~SettingsDlg()
{

}

void SettingsDlg::initialize()
{
    SettingsMgr *mgr = readerApplet->settingsMgr();

    Qt::CheckState state;

    state = mgr->showFullText() ? Qt::Checked : Qt::Unchecked;
    mShowFullTextCheck->setCheckState(state);

#ifdef _AUTO_UPDATE
    if( AutoUpdateService::instance()->shouldSupportAutoUpdate()) {
        state = AutoUpdateService::instance()->autoUpdateEnabled() ? Qt::Checked : Qt::Unchecked;
        mLatestVersionCheck->setCheckState(state);
    }
#endif

    mLangCombo->setCurrentIndex(I18NHelper::getInstance()->preferredLanguage());
}

void SettingsDlg::updateSettings()
{
    SettingsMgr *mgr = readerApplet->settingsMgr();

    mgr->setShowFullText( mShowFullTextCheck->checkState() == Qt::Checked );

#ifdef _AUTO_UPDATE
    if( AutoUpdateService::instance()->shouldSupportAutoUpdate() ) {
        bool enabled = mLatestVersionCheck->checkState() == Qt::Checked;
        AutoUpdateService::instance()->setAutoUpdateEnabled(enabled);
    }
#endif

    bool language_changed = false;

    if( mLangCombo->currentIndex() != I18NHelper::getInstance()->preferredLanguage() )
    {
        language_changed = true;
        I18NHelper::getInstance()->setPreferredLanguage(mLangCombo->currentIndex());
    }

    if( language_changed && readerApplet->yesOrNoBox(tr("You have changed language. Restart to apply it?"), this, true))
        readerApplet->restartApp();
}

void SettingsDlg::accept()
{
    updateSettings();
    QDialog::accept();
}

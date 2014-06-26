#include "mediabutton.h"

#include <settings.h>
#include <QFile>

#include <QtDebug>

MediaButton::MediaButton(QWidget *parent) :
	QPushButton(parent)
{
	this->setFlat(Settings::getInstance()->buttonsFlat());
}

/** Redefined to load custom icons saved in settings. */
void MediaButton::setIcon(const QIcon &icon, bool toggled)
{
	Settings *settings = Settings::getInstance();
	QString path;

	// Used only for play/pause behaviour. Getting the custom icon for pause can produce unexpected behaviour
	// when replacing it by play.
	if (toggled) {
		path = settings->customIcon(this, toggled);
	}

	// If the path to the custom icon has been deleted meanwhile, then delete it from settings too
	if (path.isEmpty()) {
		QPushButton::setIcon(icon);
	} else if (QFile::exists(path)) {
		QPushButton::setIcon(QIcon(path));
	} else {
		settings->setCustomIcon(this, QString());
		setIconFromTheme(settings->theme());
	}
}

/** Load an icon from a chosen theme in options. */
void MediaButton::setIconFromTheme(const QString &theme)
{
	// The objectName in the UI file MUST match the alias in the QRC file!
	QString iconFile = ":/player/" + theme.toLower() + "/" + this->objectName().remove("Button");
	QIcon icon(iconFile);
	if (!icon.isNull()) {
		this->setIcon(icon);
	}
	emit mediaButtonChanged();
}

/** Change the size of icons from the options. */
void MediaButton::setSize(const int &s)
{
	this->setIconSize(QSize(s, s));
	emit mediaButtonChanged();
}

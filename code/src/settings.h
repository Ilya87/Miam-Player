#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>

#include "mediabutton.h"

class Settings : public QSettings
{
	Q_OBJECT

private:
	/** The unique instance of this class. */
	static Settings *settings;

	/** Private constructor. */
	Settings(const QString &organization = "MmeMiamMiam", const QString & application = "MmeMiamMiamMusicPlayer")
		: QSettings(organization, application) {}

	/** Store the size of each font used in the app. */
	QMap<QString, QVariant> fontPointSizeMap;

	QList<QVariant> locations;

	Q_ENUMS(FontFamily)

public:
	enum FontFamily{PLAYLIST, LIBRARY, MENUS};

	/** Singleton Pattern to easily use Settings everywhere in the app. */
	static Settings* getInstance();

	/** Returns the actual theme name. */
	QString theme() const;

	/** Returns the actual size of media buttons. */
	int buttonSize() const;

	/** Returns true if the button in parameter is visible or not. */
	bool isVisible(MediaButton *b) const;

	/** Returns true if stars are visible and active. */
	inline bool isStarDelegates() const { return value("delegates").toBool(); }

	/** Sets the language of the application. */
	inline void setLanguage(QString lang) { setValue("language", lang); }

	/** Returns the language of the application. */
	QString language();

	/** Returns the font of the application. */
	QFont font(FontFamily fontFamily);

	/** Sets the font of the application. */
	const int fontSize(FontFamily fontFamily);

	inline bool toggleSeparators() const { return value("alphabeticalSeparators").toBool(); }

	/** Adds a new path in the application. */
	void addMusicLocation(QString location);

	/** Removes a path in the application. */
	void removeMusicLocation(QString location);

	/** Returns all music locations. */
	inline QList<QVariant> musicLocations() { return value("musicLocations").toList(); }

signals:
	void currentFontChanged();
	
public slots:
	/** Sets a new theme. */
	inline void setThemeName(const QString &theme) { setValue("theme", theme.toLower()); }

	/** Sets a new button size. */
	inline void setButtonSize(const int &s) { setValue("buttonSize", s); }

	/** Sets if the button in parameter is visible or not. */
	void setVisible(MediaButton *b, const bool &value) { setValue(b->objectName(), value); }

	/** Sets if stars are visible and active. */
	inline void setDelegates(const bool &value) { setValue("delegates", value); }

	inline void setFont(FontFamily fontFamily, const QFont &font) {
		setValue(QString(fontFamily), font.family());
		emit currentFontChanged();
	}

	inline void setFontPointSize(FontFamily fontFamily, int i) {
		fontPointSizeMap.insert(QString(fontFamily), i);
		setValue("fontPointSizeMap", fontPointSizeMap);
		emit currentFontChanged();
	}

	inline void setToggleSeparators(bool b) { setValue("alphabeticalSeparators", b); }
	
};

#endif // SETTINGS_H
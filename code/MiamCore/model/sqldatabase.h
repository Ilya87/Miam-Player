#ifndef SQLDATABASE_H
#define SQLDATABASE_H

#include "../miamcore_global.h"
#include "albumdao.h"
#include "trackdao.h"
#include "playlistdao.h"

#include <QFileInfo>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QThread>
#include <QWeakPointer>

class FileHelper;
class MusicSearchEngine;

/**
 * \brief		The SqlDatabase class uses SQLite to store few but useful tables for tracks, playlists, etc.
 * \author      Matthieu Bachelier
 * \copyright   GNU General Public License v3
 */
class MIAMCORE_LIBRARY SqlDatabase : public QObject, public QSqlDatabase
{
	Q_OBJECT
private:
	/** This worker is used to avoid a blocking UI when scanning the FileSystem. */
	QThread _workerThread;

	/** Object than can iterate throught the FileSystem for Audio files. */
	MusicSearchEngine *_musicSearchEngine;

	Q_ENUMS(extension)

public:
	explicit SqlDatabase(QObject *parent = NULL);

		enum InsertPolicy { IP_Artists			= 0,
							IP_Albums			= 1,
							IP_ArtistsAlbums	= 2,
							IP_Years			= 3 };

	virtual ~SqlDatabase() {}

	bool insertIntoTablePlaylistTracks(int playlistId, const std::list<TrackDAO> &tracks);

	int insertIntoTablePlaylists(const PlaylistDAO &playlist);

	void removePlaylists(const QList<PlaylistDAO> &playlists);

	QList<TrackDAO> selectPlaylistTracks(int playlistID);

	PlaylistDAO selectPlaylist(int playlistId);
	QList<PlaylistDAO> selectPlaylists();

	bool playlistHasBackgroundImage(int playlistID);
	void updateTablePlaylistWithBackgroundImage(int playlistID, const QString &backgroundImagePath);
	void updateTableAlbumWithCoverImage(int albumId, const QString &coverPath);

	/**
	 * Update a list of tracks. If track name has changed, it will be removed from Library then added right after.
	 * \param tracksToUpdate 'First' in pair is actual filename, 'Second' is the new filename, but may be empty.*/
	void updateTracks(const QList<QPair<QString, QString> > &tracksToUpdate);

	void loadRemoteTracks(const QList<TrackDAO> &tracks);

private:
	/** Read all tracks entries in the database and send them to connected views. */
	void loadFromFileDB();

public slots:
	/** Load an existing database file or recreate it, if not found. */
	void load();

	/** Safe delete and recreate from scratch (table Tracks only). */
	void rebuild();

private slots:
	/** Reads an external picture which is close to multimedia files (same folder). */
	void saveCoverRef(const QString &coverPath);

	/** Reads a file from the filesystem and adds it into the library. */
	void saveFileRef(const QString &absFilePath);

signals:
	void aboutToLoad();
	void coverWasUpdated(const QFileInfo &);
	void loaded();
	void progressChanged(const int &);
	// void trackExtracted(const TrackDAO &);

	void artistExtracted(const TrackDAO &);
	void albumExtracted(const AlbumDAO &);
	void trackExtracted2(const TrackDAO &);
	void nodeExtracted(GenericDAO *node, int level = 0, const QString &parent = QString());
};

#endif // SQLDATABASE_H

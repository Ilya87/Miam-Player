#include "albumdao.h"

AlbumDAO::AlbumDAO(QObject *parent)
	: GenericDAO(Miam::IT_Album, parent)
{}

AlbumDAO::AlbumDAO(const AlbumDAO &remoteAlbum) :
	GenericDAO(remoteAlbum),
	_artist(remoteAlbum.artist()),
	_artistNormalized(remoteAlbum.artistNormalized()),
	_disc(remoteAlbum.disc()),
	_cover(remoteAlbum.cover()),
	_length(remoteAlbum.length()),
	_source(remoteAlbum.source()),
	_uri(remoteAlbum.uri()),
	_year(remoteAlbum.year()),
	_artistID(remoteAlbum.artistID())
{}

AlbumDAO& AlbumDAO::operator=(const AlbumDAO& other)
{
	GenericDAO::operator=(other);
	_artist = other.artist();
	_artistID = other.artistID();
	_artistNormalized = other.artistNormalized();
	_disc = other.disc();
	_cover = other.cover();
	_length = other.length();
	_source = other.source();
	_uri = other.uri();
	_year = other.year();
	return *this;
}

AlbumDAO::~AlbumDAO() {}

uint AlbumDAO::artistID() const { return _artistID; }
void AlbumDAO::setArtistID(const uint &artistID) { _artistID = artistID; }

QString AlbumDAO::artist() const { return _artist; }
void AlbumDAO::setArtist(const QString &artist) { _artist = artist; }

QString AlbumDAO::artistNormalized() const { return _artistNormalized; }
void AlbumDAO::setArtistNormalized(const QString &artistNormalized) { _artistNormalized = artistNormalized; }

QString AlbumDAO::disc() const { return _disc; }
void AlbumDAO::setDisc(const QString &disc) { _disc = disc; }

QString AlbumDAO::cover() const { return _cover; }
void AlbumDAO::setCover(const QString &cover) { _cover = cover; }

QString AlbumDAO::length() const { return _length; }
void AlbumDAO::setLength(const QString &length) { _length = length; }

QString AlbumDAO::source() const { return _source; }
void AlbumDAO::setSource(const QString &source) { _source = source; }

QString AlbumDAO::uri() const { return _uri; }
void AlbumDAO::setUri(const QString &uri) { _uri = uri; }

QString AlbumDAO::year() const { return _year; }
void AlbumDAO::setYear(const QString &year) { _year = year; }

#include <QHash>

uint AlbumDAO::hash() const
{
	return qHash(titleNormalized()) ^ qHash(type()) ^ qHash(_year) ^ qHash(_artist);
}


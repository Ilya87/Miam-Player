#include "uniquelibrary.h"

#include "ui_uniquelibrary.h"

#include <QStandardItemModel>
#include <library/jumptowidget.h>
#include <filehelper.h>
#include <settingsprivate.h>
#include "uniquelibraryitemdelegate.h"

#include <QtDebug>

UniqueLibrary::UniqueLibrary(MediaPlayer *mediaPlayer, QWidget *parent)
	: QWidget(parent)
	, _mediaPlayer(mediaPlayer)
	, _currentTrack(nullptr)
{
	setupUi(this);
	seekSlider->setMediaPlayer(_mediaPlayer);
	connect(_mediaPlayer, &MediaPlayer::positionChanged, [=] (qint64 pos, qint64 duration) {
		if (duration > 0) {
			seekSlider->setValue(1000 * pos / duration);
		}
	});
	connect(volumeSlider, &QSlider::valueChanged, this, [=](int value) {
		_mediaPlayer->setVolume((qreal)value / 100.0);
	});
	volumeSlider->setValue(Settings::instance()->volume() * 100);

	library->setItemDelegate(new UniqueLibraryItemDelegate(library->jumpToWidget(), library->model()->proxy()));
	_proxy = library->model()->proxy();

	// Filter the library when user is typing some text to find artist, album or tracks
	connect(searchBar, &SearchBar::aboutToStartSearch, library->model()->proxy(), &UniqueLibraryFilterProxyModel::findMusic);
	connect(library, &TableView::doubleClicked, this, &UniqueLibrary::playSingleTrack);

	connect(skipBackwardButton, &MediaButton::clicked, this, &UniqueLibrary::skipBackward);
	connect(seekBackwardButton, &MediaButton::clicked, _mediaPlayer, &MediaPlayer::seekBackward);
	connect(playButton, &MediaButton::clicked, _mediaPlayer, &MediaPlayer::play);
	connect(stopButton, &MediaButton::clicked, _mediaPlayer, &MediaPlayer::stop);
	connect(seekForwardButton, &MediaButton::clicked, _mediaPlayer, &MediaPlayer::seekForward);
	connect(skipForwardButton, &MediaButton::clicked, this, &UniqueLibrary::skipForward);
	connect(toggleShuffleButton, &MediaButton::clicked, this, &UniqueLibrary::toggleShuffle);

	connect(_mediaPlayer, &MediaPlayer::stateChanged, this, [=](QMediaPlayer::State state) {
		if (_currentTrack) {
			if (state == QMediaPlayer::StoppedState) {
				_currentTrack->setData(false, Miam::DF_Highlighted);
			} else if (state == QMediaPlayer::PlayingState) {
				_currentTrack->setData(true, Miam::DF_Highlighted);
			}
		}
	});

	connect(_mediaPlayer, &MediaPlayer::positionChanged, this, [=](qint64 pos, qint64) {
		if (_currentTrack) {
			uint p = pos / 1000;
			_currentTrack->setData(p, Miam::DF_CurrentPosition);
		}
	});

	auto settings = SettingsPrivate::instance();
	connect(settings, &SettingsPrivate::languageAboutToChange, this, [=](const QString &newLanguage) {
		QApplication::removeTranslator(&translator);
		translator.load(":/uniqueLibrary_" + newLanguage);
		QApplication::installTranslator(&translator);
	});

	// Init language
	translator.load(":/uniqueLibrary_" + settings->language());
	QApplication::installTranslator(&translator);
}

void UniqueLibrary::changeEvent(QEvent *event)
{
	if (event->type() == QEvent::LanguageChange) {
		this->retranslateUi(this);
	} else {
		QWidget::changeEvent(event);
	}
}

bool UniqueLibrary::playSingleTrack(const QModelIndex &index)
{
	qDebug() << Q_FUNC_INFO << index;
	if (_currentTrack) {
		_currentTrack->setData(false, Miam::DF_Highlighted);
	}
	QStandardItem *item = library->model()->itemFromIndex(_proxy->mapToSource(index));
	if (item && item->type() == Miam::IT_Track) {
		qDebug() << Q_FUNC_INFO << "about to play" << index.data(Miam::DF_URI).toString();
		_mediaPlayer->playMediaContent(QUrl::fromLocalFile(index.data(Miam::DF_URI).toString()));
		_currentTrack = item;
		return true;
	} else {
		return false;
	}
}

void UniqueLibrary::skipBackward()
{
	qDebug() << Q_FUNC_INFO << "not yet implemented";
	if (!_currentTrack) {
		return;
	}
}

void UniqueLibrary::skipForward()
{
	if (!_currentTrack) {
		return;
	}
	QModelIndex current = _proxy->mapFromSource(library->model()->index(_currentTrack->row(), 1));
	int row = current.row();
	while (row < library->model()->rowCount()) {
		qDebug() << Q_FUNC_INFO << row;
		QModelIndex next = current.sibling(row + 1, 1);
		if (this->playSingleTrack(next)) {
			break;
		} else {
			row++;
		}
	}
}

void UniqueLibrary::toggleShuffle()
{
	qDebug() << Q_FUNC_INFO << "not yet implemented";
}

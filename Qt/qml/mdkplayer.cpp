#include "mdkplayer.h"
#include <QDebug>
#include <QDir>
#include <QImage>

class VideoRendererInternal : public QQuickFramebufferObject::Renderer
{
public:
    VideoRendererInternal(QmlMDKPlayer *r)
        : save_image_flag_(false)
    {
        this->r = r;
    }

    void render() override {
        r->renderVideo();
        if(save_image_flag_)
        {
            qDebug() << "toImage and save" << img_path_;
            save_image_flag_ = false;
            framebufferObject()->toImage(false).save(img_path_);
        }
    }

    QOpenGLFramebufferObject *createFramebufferObject(const QSize &size) override {
        r->setVideoSurfaceSize(frame_size_.width(), frame_size_.height());
        return new QOpenGLFramebufferObject(frame_size_);
    }

    void saveImage(const QString &img_path) {
        img_path_ = img_path;
        save_image_flag_ = true;
        update();
    }

    QmlMDKPlayer *r;
    bool save_image_flag_;
    QString img_path_;

    QSize frame_size_;
};


QmlMDKPlayer::QmlMDKPlayer(QQuickItem *parent):
    QQuickFramebufferObject(parent),
    internal_player(new Player),
    renderer_(nullptr)
{
    this->setTextureFollowsItemSize(false);
}

QmlMDKPlayer::~QmlMDKPlayer()
{
    delete internal_player;
}

QQuickFramebufferObject::Renderer *QmlMDKPlayer::createRenderer() const
{
    renderer_ = new VideoRendererInternal(const_cast<QmlMDKPlayer*>(this));
    auto media_info = internal_player->mediaInfo();
    if(!media_info.video.empty())
    {
        QSize frame_size = QSize(media_info.video[0].codec.width, media_info.video[0].codec.height);
        static_cast<VideoRendererInternal*>(renderer_)->frame_size_ = frame_size;
    }
    return renderer_;
}

void QmlMDKPlayer::play()
{
    qDebug() << "play";
    internal_player->setState(PlaybackState::Playing);
    internal_player->scale(1.0f, -1.0f);
    internal_player->setRenderCallback([=](void *){
        QMetaObject::invokeMethod(this, "update");
    });
}

void QmlMDKPlayer::setPlaybackRate(float rate)
{
    internal_player->setPlaybackRate(rate);
}

void QmlMDKPlayer::setVideoSurfaceSize(int width, int height)
{
    internal_player->setVideoSurfaceSize(width, height);
}

void QmlMDKPlayer::snapshot(const QString &dst)
{
    qDebug() << "snapshot";
    QString img_dir = dst;
    if(img_dir.isEmpty())
    {
        img_dir = QDir::homePath() + QStringLiteral("/Pictures/qmlplayer/snapshot");
        if(!QDir().mkpath(img_dir))
        {
            qDebug() << "Save snapshot error: failed to mkpath:" << img_dir;
            // TODO: error msg
            return;
        }
    }
    static_cast<VideoRendererInternal*>(renderer_)->saveImage(img_dir + QStringLiteral("/snapshot.jpg"));
}

void QmlMDKPlayer::renderVideo()
{
    internal_player->renderVideo();
}

#include <QtGui/QtGui>
#include <QApplication>

#include <QGraphicsScene>
#include <QGraphicsBlurEffect>
#include <QGraphicsPixmapItem>

#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QSlider>

#include <QDir>
#include <QFileDialog>


QImage blurImage(QImage source,int blurRadius)
{
    if(source.isNull()) return QImage();
    QGraphicsScene scene;
    QGraphicsPixmapItem item;
    item.setPixmap(QPixmap::fromImage(source));
    //
    auto *blur = new QGraphicsBlurEffect;
    blur->setBlurRadius(blurRadius);
    item.setGraphicsEffect(blur);
    scene.addItem(&item);
    QImage result(source.size(),QImage::Format_ARGB32);
    result.fill(Qt::transparent);
    QPainter painter(&result);
    scene.render(&painter, QRectF(),
                 QRectF(0,0,source.width(),source.height()));
    return result;
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QWidget mainWindow;

    QImage sourceImage;

    auto *imageLabel = new QLabel(&mainWindow);
    auto *slider = new QSlider(&mainWindow);
    auto *openFileButton = new QPushButton("Open",&mainWindow);
    auto *vbox = new QVBoxLayout(&mainWindow);

    slider->setMinimum(0);
    slider->setMaximum(10);

    QObject::connect(openFileButton,&QPushButton::clicked,[&sourceImage,imageLabel]
    {
        QString filePath = QFileDialog::getOpenFileName(nullptr,
                                     "Open mp3 song",
                                     "F:\\public\\alx\\skilbox_projects_Qt\\383_BlurImage",
                                     "Jpeg files (*.jpg)");

        sourceImage.load(filePath);

        imageLabel->setPixmap(QPixmap::fromImage(sourceImage).scaled(
                                            imageLabel->width(),
                                            imageLabel->height(), Qt::KeepAspectRatio));

    });


    QObject::connect(slider,&QSlider::valueChanged,[imageLabel,&sourceImage](qint64 newValue)
    {
        imageLabel->setPixmap(QPixmap::fromImage(blurImage(sourceImage, newValue).scaled(
                                            imageLabel->width(),
                                            imageLabel->height(), Qt::KeepAspectRatio)));

    });


    slider->setOrientation(Qt::Horizontal);

    vbox->addWidget(imageLabel);
    vbox->addWidget(slider);
    vbox->addWidget(openFileButton);

    mainWindow.resize(480, 640);
    mainWindow.show();

    return a.exec();
}

#include <qdir.h>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QQmlEngine>

#include "FaceDetector.h"
#include "FaceDetectorBlock.h"
#include "FileSourceBlock.h"
#include "QBackend.h"
#include "ResultBlock.h"



int main(int argc, char *argv[])
{
#if defined(Q_OS_WIN)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);


	QQuickStyle::setStyle("Material");
	
    QQmlApplicationEngine engine;
    
   
	
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
	
    if (engine.rootObjects().isEmpty())
        return -1;
	
    QQmlContext* context = engine.rootContext();
    QBackend backend;
    context->setContextProperty("_backend", &backend);
 
    

    return app.exec();
}

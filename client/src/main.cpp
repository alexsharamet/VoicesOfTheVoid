#include <QGuiApplication>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQuickView>

#include "logic/network.h"
#include "ui/mainPageModel.h"
#include "ui/mainPageViewModel.h"
#include "utils/config.h"

void loadConfig() {
    Utils::Config::instance().load();
    Utils::Config::instance().save();
}

Logic::Network *createNetwork() {
    auto network = new Logic::Network();
    network->setConnection(Utils::Config::instance().getHost(),
                           Utils::Config::instance().getPort());

    return network;
}

QQuickView *createMainView(QGuiApplication &app) {
    auto view = new QQuickView();
    view->setSource(QUrl("qrc:/res/ui/form.qml"));

    QQuickView::connect(view->engine(), &QQmlEngine::quit, &app, &QCoreApplication::quit);
    view->setResizeMode(QQuickView::SizeRootObjectToView);
    return view;
}

UI::MainPageModel *createMainPageModel(Logic::Network *network) {
    auto mainPageModel = new UI::MainPageModel(network);
    mainPageModel->auth();
    return mainPageModel;
}

UI::MainPageViewModel *createMainPageViewModel(QQuickView *view, UI::MainPageModel *model) {
    auto viewModel = new UI::MainPageViewModel(*view, "mainPageViewModel");

    QObject::connect(viewModel, &UI::MainPageViewModel::outputChanged, model, [viewModel, model] {
        model->message(viewModel->output());
    });

    QObject::connect(model, &UI::MainPageModel::gotMessage, viewModel, [viewModel, model](QString text) {
        viewModel->setInput(text);
    });

    return viewModel;
}

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);
    QCoreApplication::setOrganizationName("VoicesOfTheVoid");
    QCoreApplication::setApplicationName("client");

    loadConfig();
    auto network = createNetwork();
    auto mainPageModel = createMainPageModel(network);
    auto mainView = createMainView(app);
    [[maybe_unused]] auto mainPageViewModel = createMainPageViewModel(mainView, mainPageModel);

    if (mainView->status() == QQuickView::Error) {
        return -1;
    }
    mainView->show();
    return QGuiApplication::exec();
}
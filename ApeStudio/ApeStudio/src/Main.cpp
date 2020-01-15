#include "MainWindow.hpp"
#include "TableModel.hpp"

#include <Rave/RaveCore/RaveAssetBuilder.hpp>
#include <Rave/RaveCore/RaveEffectCollectionReader.hpp>
#include <Rave/RaveCore/RaveInputHandler.hpp>
#include <Rave/RaveCore/RaveSceneBuilder.hpp>
#include <Rave/RaveCore/RaveSkyboxCollectionReader.hpp>

#include <Ape/Engine/QtEngine/QtEngine.hpp>
#include <Ape/Engine/QtEngine/QtGateway.hpp>
#include <Ape/Engine/QtEngine/QtWindow.hpp>
#include <Ape/Rendering/Effect/EffectCollection.hpp>
#include <Ape/Rendering/Effect/EffectSelector.hpp>
#include <Ape/Rendering/Lighting/MonoDepthShaderProgram.hpp>
#include <Ape/Rendering/Lighting/OmniDepthCubeShaderProgram.hpp>
#include <Ape/Rendering/Lighting/OmniDepthFlatShaderProgram.hpp>
#include <Ape/Rendering/Lighting/BlinnPhongShaderProgram.hpp>
#include <Ape/Rendering/Rendering/BodyBoundsShaderProgram.hpp>
#include <Ape/Rendering/Rendering/SceneRenderer.hpp>
#include <Ape/Rendering/Skybox/SkyboxCollection.hpp>
#include <Ape/Rendering/Skybox/SkyboxSelector.hpp>
#include <Ape/Rendering/Skybox/SkyboxShaderProgram.hpp>
#include <Ape/Rendering/Wireframe/LineStyleProvider.hpp>
#include <Ape/Rendering/Wireframe/WireframeShaderProgram.hpp>
#include <Ape/World/Scene/BodySelector.hpp>
#include <Ape/World/Scene/CameraSelector.hpp>
#include <Ape/World/Shape/ShapeArrayObjectDrawer.hpp>
#include <Ape/World/Shape/ShapeBufferObjectDrawer.hpp>

#include <Basix/Range/Search.hpp>

#include <QApplication>
#include <QFileSystemModel>
#include <QGridLayout>
#include <QListView>
#include <QScreen>
#include <QTableView>
#include <QTreeView>
#include <QWidget>

#include <iostream>

template<typename Model>
auto makeListView(QWidget & parent, Model & model)
    -> QListView &
{
    auto view = new QListView{&parent};

    view->setModel(&model);

    return *view;
}

template<typename Model>
auto makeTableView(QWidget & parent, Model & model)
    -> QTableView &
{
    auto view = new QTableView{&parent};

    view->setModel(&model);

    return *view;
}

template<typename Model>
auto makeTreeView(QWidget & parent, Model & model)
    -> QTreeView &
{
    auto view = new QTreeView{&parent};

    view->setModel(&model);

    return *view;
}

auto makeQtWindow(ape::qt::QtGateway & gateway, QWidget & parent)
    -> ape::qt::QtWindow &
{
    auto const widget = gateway.createWindow(parent);

    auto format = QSurfaceFormat{};

    format.setColorSpace(QSurfaceFormat::ColorSpace::sRGBColorSpace);

    format.setVersion(4, 5);

    widget->setFormat(format);

    widget->setTextureFormat(GL_SRGB8_ALPHA8);

    return *widget;
}

auto makeCentralWidget()
    -> QWidget &
{
    auto widget = new QWidget{};

    auto layout = new QGridLayout{widget};

    widget->setLayout(layout);

    return *widget;
}

auto getGridLayout(QWidget & widget)
    -> QGridLayout &
{
    return static_cast<QGridLayout &>(*widget.layout());
}

auto isDebugOutputEnabled(std::vector<std::string> const & arguments)
    -> bool
{
    return basix::contains(arguments, "--enable-debug-output");
}

auto isSponzaExcluded(std::vector<std::string> const & arguments)
    -> bool
{
    return basix::contains(arguments, "--exclude-sponza");
}

int main(int argc, char *argv[])
{
    auto const arguments = std::vector<std::string>{argv, argv + argc};

    auto const enableDebugOutput = isDebugOutputEnabled(arguments);

    auto const doNotIncludeSponza = isSponzaExcluded(arguments);

    auto gateway = ape::qt::QtGateway{enableDebugOutput};

    auto && app = QApplication{argc, argv};
    
    auto && model = TableModel{2, 3};

    auto && window = MainWindow{model};

    auto && fsModel = QFileSystemModel{};

    fsModel.setRootPath(QDir::currentPath());

    auto & centralWidget = makeCentralWidget();

    window.setCentralWidget(&centralWidget);

    auto & tableView1 = makeTableView(centralWidget, model);

    auto & treeView1 = makeTreeView(centralWidget, model);

    auto & treeView2 = makeTreeView(centralWidget, fsModel);

    treeView2.setRootIndex(fsModel.index(QString{"c:\\temp"}));

    auto & listView1 = makeListView(centralWidget, model);

    auto & sceneView1 = makeQtWindow(gateway, window);

    sceneView1.setFocusPolicy(Qt::FocusPolicy::ClickFocus);

    auto & sceneView2 = makeQtWindow(gateway, window);

    sceneView2.setFocusPolicy(Qt::FocusPolicy::ClickFocus);

    auto & sceneView3 = makeQtWindow(gateway, window);

    sceneView3.setFocusPolicy(Qt::FocusPolicy::ClickFocus);

    getGridLayout(centralWidget).addWidget(&tableView1, 0, 0);

    getGridLayout(centralWidget).addWidget(&sceneView1, 0, 1);

    getGridLayout(centralWidget).addWidget(&treeView1, 0, 2);

    getGridLayout(centralWidget).addWidget(&sceneView2, 1, 0);

    getGridLayout(centralWidget).addWidget(&listView1, 1, 1);

    getGridLayout(centralWidget).addWidget(&sceneView3, 1, 2);
    
    getGridLayout(centralWidget).setColumnStretch(0, 1.0);
    
    getGridLayout(centralWidget).setColumnStretch(1, 1.0);
    
    getGridLayout(centralWidget).setColumnStretch(2, 1.0);

    treeView1.setSelectionModel(tableView1.selectionModel());

    treeView2.setSelectionModel(tableView1.selectionModel());

    listView1.setSelectionModel(tableView1.selectionModel());

    auto const screenSize = QApplication::screens().front()->size();

    window.resize(screenSize.width(), screenSize.height());

    window.registerQtWindow(sceneView1);

    window.registerQtWindow(sceneView2);

    window.registerQtWindow(sceneView3);

    window.show();

    auto const sharing = QOpenGLContext::areSharing(sceneView1.context(), sceneView2.context());

    std::cout << "Sharing is " << (sharing ? "enabled" : "disabled") << std::endl;

    sceneView1.makeCurrent();

    auto assets = rave::createRaveAssets(doNotIncludeSponza);
    
    auto scene = rave::createRaveScene(assets, doNotIncludeSponza);

    auto standardShader = ape::BlinnPhongShaderProgram{};

    auto monoDepthShader = ape::MonoDepthShaderProgram{};

    auto omniDepthCubeShader = ape::OmniDepthCubeShaderProgram{};

    auto omniDepthFlatShader = ape::OmniDepthFlatShaderProgram{};

    auto wireframeShader = ape::WireframeShaderProgram{};

    auto boundsShader = ape::BodyBoundsShaderProgram{};

    auto skyboxShader = ape::SkyboxShaderProgram{};

    auto effectCollection = rave::RaveEffectCollectionReader{}.read();

    auto skyboxCollection = rave::RaveSkyboxCollectionReader{}.read();

    auto picker = ape::BodySelector{scene};

    auto wireframeStyleProvider = ape::LineStyleProvider{{0.05f, {0.2f, 0.2f, 1.0f}}};

    auto const backgroundColor = glm::vec3{0.0f, 0.0f, 0.0f};
    
    /// ---

    auto cameraSelector1 = ape::CameraSelector{scene};

    // IMPORTANT: fallback VAO in renderer as well as VAOs in ShapeDrawer must be created in the
    // corresponding rendering context! Also, the flat quad VAO for rendering of offscreen texture
    // must be created in the corresponding renderer context.
    sceneView1.makeCurrent();
    
    // Using a VAO per shape seems to make performance worse...
    //auto shapeRenderer1 = std::make_unique<ape::ShapeArrayObjectDrawer>(assets.shapes);
    auto shapeRenderer1 = std::make_unique<ape::ShapeBufferObjectDrawer>();

    auto depthBodyRenderer1 = ape::DepthBodyRenderer{
        {monoDepthShader, *shapeRenderer1},
        {omniDepthCubeShader, *shapeRenderer1},
        {omniDepthFlatShader, *shapeRenderer1}};

    auto blinnPhongRenderer1 = ape::BlinnPhongBodyRenderer{standardShader, *shapeRenderer1};

    auto wireframeBodyRenderer1 = ape::WireframeBodyRenderer{
        wireframeShader,
        *shapeRenderer1,
        wireframeStyleProvider};

    auto outlinedBodyRenderer1 = ape::OutlinedBodyRenderer{
        blinnPhongRenderer1,
        wireframeBodyRenderer1};

    auto boundsRenderer1 = ape::BodyBoundsRenderer{boundsShader};

    auto skyboxSelector1 = ape::SkyboxSelector{skyboxCollection};

    auto skyboxRenderer1 = ape::SkyboxRenderer{skyboxShader, skyboxSelector1};

    auto effectSelector1 = ape::EffectSelector{effectCollection};

    auto effectRenderer1 = ape::EffectRenderer{effectSelector1};

    auto renderer1 = ape::SceneRenderer{
        std::move(shapeRenderer1),
        ape::SceneRenderer::RendererSet{
            std::move(depthBodyRenderer1),
            std::move(blinnPhongRenderer1),
            std::move(wireframeBodyRenderer1),
            std::move(outlinedBodyRenderer1),
            std::move(boundsRenderer1),
            std::move(skyboxRenderer1),
            std::move(effectRenderer1)},
        cameraSelector1,
        picker,
        sceneView1,
        ape::Viewport{{0, 0}, sceneView1.getSize()},
        backgroundColor};

    auto inputHandler1 = rave::RaveInputHandler{
        sceneView1,
        renderer1,
        cameraSelector1,
        skyboxSelector1,
        effectSelector1,
        picker,
        standardShader,
        wireframeStyleProvider,
        scene};

    auto engine1 = ape::qt::QtEngine{sceneView1, renderer1, inputHandler1};

    engine1.run();

    /// ---

    auto cameraSelector2 = ape::CameraSelector{scene};

    // IMPORTANT: fallback VAO in renderer as well as VAOs in ShapeDrawer must be created in the
    // corresponding rendering context! Also, the flat quad VAO for rendering of offscreen texture
    // must be created in the corresponding renderer context.
    sceneView2.makeCurrent();

    // Using a VAO per shape seems to make performance worse...
    //auto shapeRenderer2 = std::make_unique<ape::ShapeArrayObjectDrawer>(assets.shapes);
    auto shapeRenderer2 = std::make_unique<ape::ShapeBufferObjectDrawer>();

    auto depthBodyRenderer2 = ape::DepthBodyRenderer{
        {monoDepthShader, *shapeRenderer2},
        {omniDepthCubeShader, *shapeRenderer2},
        {omniDepthFlatShader, *shapeRenderer2}};

    auto blinnPhongRenderer2 = ape::BlinnPhongBodyRenderer{standardShader, *shapeRenderer2};

    auto wireframeBodyRenderer2 = ape::WireframeBodyRenderer{
        wireframeShader,
        *shapeRenderer2,
        wireframeStyleProvider};

    auto outlinedBodyRenderer2 = ape::OutlinedBodyRenderer{
        blinnPhongRenderer2,
        wireframeBodyRenderer2};
    
    auto boundsRenderer2 = ape::BodyBoundsRenderer{boundsShader};

    auto skyboxSelector2 = ape::SkyboxSelector{skyboxCollection};

    auto skyboxRenderer2 = ape::SkyboxRenderer{skyboxShader, skyboxSelector2};

    auto effectSelector2 = ape::EffectSelector{effectCollection};

    auto effectRenderer2 = ape::EffectRenderer{effectSelector2};

    auto renderer2 = ape::SceneRenderer{
        std::move(shapeRenderer2),
        ape::SceneRenderer::RendererSet{
            std::move(depthBodyRenderer2),
            std::move(blinnPhongRenderer2),
            std::move(wireframeBodyRenderer2),
            std::move(outlinedBodyRenderer2),
            std::move(boundsRenderer2),
            std::move(skyboxRenderer2),
            std::move(effectRenderer2)},
        cameraSelector2,
        picker,
        sceneView2,
        ape::Viewport{{0, 0}, sceneView2.getSize()},
        backgroundColor};

    auto inputHandler2 = rave::RaveInputHandler{
        sceneView2,
        renderer2,
        cameraSelector2,
        skyboxSelector2,
        effectSelector2,
        picker,
        standardShader,
        wireframeStyleProvider,
        scene};

    auto engine2 = ape::qt::QtEngine{sceneView2, renderer2, inputHandler2};

    effectSelector2.activateNextEffect();

    cameraSelector2.activateNextCamera();

    engine2.run();
    
    /// ---
    
    auto cameraSelector3 = ape::CameraSelector{scene};

    // IMPORTANT: fallback VAO in renderer as well as VAOs in ShapeDrawer must be created in the
    // corresponding rendering context! Also, the flat quad VAO for rendering of offscreen texture
    // must be created in the corresponding renderer context.
    sceneView3.makeCurrent();

    // Using a VAO per shape seems to make performance worse...
    //auto shapeRenderer3 = std::make_unique<ape::ShapeArrayObjectDrawer>(assets.shapes);
    auto shapeRenderer3 = std::make_unique<ape::ShapeBufferObjectDrawer>();

    auto depthBodyRenderer3 = ape::DepthBodyRenderer{
        {monoDepthShader, *shapeRenderer3},
        {omniDepthCubeShader, *shapeRenderer3},
        {omniDepthFlatShader, *shapeRenderer3}};

    auto blinnPhongRenderer3 = ape::BlinnPhongBodyRenderer{standardShader, *shapeRenderer3};

    auto wireframeBodyRenderer3 = ape::WireframeBodyRenderer{
        wireframeShader,
        *shapeRenderer3,
        wireframeStyleProvider};

    auto outlinedBodyRenderer3 = ape::OutlinedBodyRenderer{
        blinnPhongRenderer3,
        wireframeBodyRenderer3};

    auto boundsRenderer3 = ape::BodyBoundsRenderer{boundsShader};

    auto skyboxSelector3 = ape::SkyboxSelector{skyboxCollection};

    auto skyboxRenderer3 = ape::SkyboxRenderer{skyboxShader, skyboxSelector3};

    auto effectSelector3 = ape::EffectSelector{effectCollection};

    auto effectRenderer3 = ape::EffectRenderer{effectSelector3};

    auto renderer3 = ape::SceneRenderer{
        std::move(shapeRenderer3),
        ape::SceneRenderer::RendererSet{
            std::move(depthBodyRenderer3),
            std::move(blinnPhongRenderer3),
            std::move(wireframeBodyRenderer3),
            std::move(outlinedBodyRenderer3),
            std::move(boundsRenderer3),
            std::move(skyboxRenderer3),
            std::move(effectRenderer3)},
        cameraSelector3,
        picker,
        sceneView3,
        ape::Viewport{{0, 0}, sceneView3.getSize()},
        backgroundColor};

    auto inputHandler3 = rave::RaveInputHandler{
        sceneView3,
        renderer3,
        cameraSelector3,
        skyboxSelector3,
        effectSelector3,
        picker,
        standardShader,
        wireframeStyleProvider,
        scene};

    auto engine3 = ape::qt::QtEngine{sceneView3, renderer3, inputHandler3};

    effectSelector3.activatePreviousEffect();

    cameraSelector3.activatePreviousCamera();

    engine3.run();
    
    // ---

    return app.exec();
}

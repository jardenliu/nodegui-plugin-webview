#include "qwebengineview_wrap.h"

#include <nodegui/Extras/Utils/nutils.h>
#include <nodegui/QtWidgets/QWidget/qwidget_wrap.h>

#include <QUrl>
#include <QWidget>
#include <QtWebEngine>

Napi::FunctionReference QWebEngineViewWrap::constructor;

Napi::Object QWebEngineViewWrap::init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);
  char CLASSNAME[] = "QWebEngineView";
  Napi::Function func = DefineClass(
      env, CLASSNAME,
      {// InstanceMethod("showMessage", &QWebEngineViewWrap::showMessage),
       QWIDGET_WRAPPED_METHODS_EXPORT_DEFINE(QWebEngineViewWrap)});
  constructor = Napi::Persistent(func);
  exports.Set(CLASSNAME, func);
  QtWebEngine::initialize();
  return exports;
}

NWebEngineView *QWebEngineViewWrap::getInternalInstance() {
  return this->instance;
}

QWebEngineViewWrap::QWebEngineViewWrap(const Napi::CallbackInfo &info)
    : Napi::ObjectWrap<QWebEngineViewWrap>(info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  if (info.Length() == 1) {
    Napi::Object parentObject = info[0].As<Napi::Object>();
    QWidgetWrap *parentWidgetWrap =
        Napi::ObjectWrap<QWidgetWrap>::Unwrap(parentObject);
    this->instance =
        new NWebEngineView(parentWidgetWrap->getInternalInstance());
  } else if (info.Length() == 0) {
    this->instance = new NWebEngineView();
  } else {
    Napi::TypeError::New(env, "Wrong number of arguments")
        .ThrowAsJavaScriptException();
  }
  this->rawData = extrautils::configureQWidget(
      this->getInternalInstance(), this->getInternalInstance()->getFlexNode(),
      true);
}

QWebEngineViewWrap::~QWebEngineViewWrap() {
  extrautils::safeDelete(this->instance);
}

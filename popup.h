#ifndef POPUP_H
#define POPUP_H

#include <QGridLayout>
#include <QLabel>
#include <QPropertyAnimation>
#include <QScreen>
#include <QTimer>
#include <QWidget>

class PopUp : public QWidget {
  Q_OBJECT

  // Свойство полупрозрачности
  Q_PROPERTY(float popupOpacity READ getPopupOpacity WRITE setPopupOpacity)

  void setPopupOpacity(float opacity);
  float getPopupOpacity() const;

public:
  explicit PopUp(bool isError = false, QWidget *parent = 0);

protected:
  void paintEvent(
      QPaintEvent *event); // Фон будет отрисовываться через метод перерисовки

public slots:
  void setPopupText(const QString &text); // Установка текста в уведомление
  void show(); /* Собственный метод показа виджета
                * Необходимо для преварительной настройки анимации
                * */

private slots:
  void hideAnimation(); // Слот для запуска анимации скрытия
  void hide(); /* По окончании анимации, в данном слоте делается проверка,
                * виден ли виджет, или его необходимо скрыть
                * */

private:
  bool isError = false;
  QLabel label;                 // Label с сообщением
  QGridLayout layout;           // Размещение для лейбла
  QPropertyAnimation animation; // Свойство анимации для всплывающего сообщения
  float popupOpacity; // Свойства полупрозрачности виджета
  QTimer *timer; // Таймер, по которому виджет будет скрыт
};

#endif // POPUP_H

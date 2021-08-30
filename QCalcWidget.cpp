#include "QCalcWidget.h"
#include "ui_QCalcWidget.h"
#include <iostream>


QCalcWidget::QCalcWidget( QWidget* parent ) : QWidget( parent ), vUi( new Ui::QCalcWidget )
{
  vUi->setupUi( this );

  resetLineEdit();

  auto defaultPalette = vUi->mHistory->palette();
  defaultPalette.setColor( QPalette::Base, defaultPalette.color( QPalette::Window ) );
  vUi->mHistory->setPalette( defaultPalette );
  vUi->mHistory->setFrame( false );

  std::map<QToolButton*, int> numberButtons{ { vUi->mButton_0, 0 }, { vUi->mButton_1, 1 },
                                             { vUi->mButton_2, 2 }, { vUi->mButton_3, 3 },
                                             { vUi->mButton_4, 4 }, { vUi->mButton_5, 5 },
                                             { vUi->mButton_6, 6 }, { vUi->mButton_7, 7 },
                                             { vUi->mButton_8, 8 }, { vUi->mButton_9, 9 } };

  for ( auto& button : numberButtons ) {
    connect( button.first, &QAbstractButton::pressed, this, [&, button]() {
      numberButtonPushed( button.second );
    } );
  }

  connect( vUi->mButton_Clear, &QAbstractButton::pressed, this, [&]() { clearButtonPushed(); } );

  std::vector<std::tuple<QToolButton*, QString, Operations>> opsButtons{
    { vUi->mButton_Plus, " + ", Plus },
    { vUi->mButton_Multiply, " x ", Multiply },
    { vUi->mButton_Divide, " / ", Divide },
    { vUi->mButton_Subtract, " - ", Minus }
  };

  for ( auto& button : opsButtons ) {
    connect( std::get<0>( button ), &QAbstractButton::pressed, this, [&, button]() {
      moveTextToHistory( std::get<1>( button ) );
      operation = std::get<2>( button );
    } );
  }

  connect( vUi->mButton_Answer, &QAbstractButton::pressed, this, [&]() { calculateAnswer(); } );
}

QCalcWidget::~QCalcWidget() = default;


void QCalcWidget::numberButtonPushed( int number )
{
  if ( number == 0 && vUi->mLineEdit->text() == "0" ) {
    // Do Nothing here maybe?
  } else {
    if ( vUi->mLineEdit->text() == "0" ) {
      vUi->mLineEdit->clear();
    }
    vUi->mLineEdit->setText( vUi->mLineEdit->text() + QString( "%1" ).arg( number ) );
  }
}

void QCalcWidget::clearButtonPushed()
{
  resetLineEdit();
  vUi->mHistory->clear();
}

void QCalcWidget::moveTextToHistory( QString ops )
{
  QString number = vUi->mLineEdit->text();
  firstNumber = number.toInt();
  vUi->mHistory->setText( number + ops );
  resetLineEdit();
}

void QCalcWidget::resetLineEdit()
{
  vUi->mLineEdit->setText( "0" );
}

void QCalcWidget::calculateAnswer()
{
  QString number = vUi->mLineEdit->text();
  secondNumber = number.toInt();
  vUi->mHistory->setText( vUi->mHistory->text() + number + " = " );

  int answer = 0;
  switch ( operation ) {
    case Plus:
      answer = firstNumber + secondNumber;
      break;
    case Minus:
      answer = firstNumber - secondNumber;

      break;
    case Multiply:
      answer = firstNumber * secondNumber;

      break;
    case Divide:
      answer = firstNumber / secondNumber;

      break;
    case None:
      break;
  }

  vUi->mLineEdit->setText( QString( "%1" ).arg( answer ) );
}

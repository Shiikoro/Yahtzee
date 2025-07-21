#include "dicewidget.h"
#include <stdlib.h>

#define BORDER 5
#define MIN_DICE_SIZE 50
#define STD_DICE_SIZE 100

DiceWidget::DiceWidget( QWidget* parent )
    : QWidget( parent ), value( 0 ), clickStarted( false ),
      mayRoll( true ), mayChange(false)
{
    setMinimumSize( MIN_DICE_SIZE, MIN_DICE_SIZE );
    rollDice();
}

QSize DiceWidget::sizeHint( void ) const
{
    return QSize( STD_DICE_SIZE, STD_DICE_SIZE );
}

void DiceWidget::paintEvent( QPaintEvent* /*event*/ )
{
    QColor fgColor = palette().color( QPalette::Text );
    QColor bgColor;
    if(!mayChange) bgColor = palette().color( QPalette::Link );
    else bgColor = palette().color( mayRoll ? QPalette::Base : QPalette:: AlternateBase );
    QPainter painter( this );
    painter.setRenderHint( QPainter::Antialiasing );

    painter.setPen( fgColor );
    painter.setBrush( bgColor );
    painter.drawRoundedRect( diceRect, 42, 42, Qt::RelativeSize );

    // Punkte zeichnen
    painter.setBrush( fgColor );
    painter.translate( rect().center() );
    switch( value )
    {
    case 5:
        drawTopLeftBottomRightDots( painter );
    case 3:
        drawBottomLeftTopRightDots( painter );
    case 1:
        drawCenterDot( painter );
        break;
    case 6:
        drawSideMiddleDots( painter );
    case 4:
        drawBottomLeftTopRightDots( painter );
    case 2:
        drawTopLeftBottomRightDots( painter );
        break;
    }
}

void DiceWidget::mousePressEvent( QMouseEvent* event )
{
    // Kontrolle, ob _linke_ Taste gedrückt
    if( event->button() != Qt::LeftButton ) return;
    // Klickbaren Bereich ermitteln
    // -> passiert im resizeEvent() und ist jetzt diceRect
    // Merken, ob im im klickbaren Bereich gedrückt wurde
    clickStarted = diceRect.contains( event->pos() );
}

void DiceWidget::mouseReleaseEvent( QMouseEvent* event )
{
    // Kontrolle, ob _linke_ Taste losgelassen
    if( event->button() != Qt::LeftButton ) return;
    // Kontrolle, ob Klick überhaupt richtig gestartet wurde
    if( !clickStarted ) return;
    // Klickbaren Bereich ermitteln
    // -> passiert im resizeEvent() und ist jetzt diceRect
    // Kontrolle, ob im klickbaren Bereich auch losgelassen
    if( diceRect.contains( event->pos() ) )
        emit clicked();
    clickStarted = false;
}

int DiceWidget::getValue( void ) const
{
    return value;
}

void DiceWidget::resizeEvent( QResizeEvent* /*event*/ )
{
    diceSize = width() < height() ? width() : height();
    diceSize -= 2 * BORDER;
    dotRadius = diceSize / 14.0;
    diceRect = QRect( 0, 0, diceSize, diceSize );
    diceRect.moveCenter( rect().center() );
}

bool DiceWidget::getMayRoll( void ) const
{
    return mayRoll;
}

void DiceWidget::setMayRoll( bool newState )
{
    if(!mayChange) return;
    if( mayRoll == newState ) return;
    mayRoll = newState;
    update();
    emit mayRollChanged( newState );
}

void DiceWidget::setMayRollOn( void )
{
    setMayRoll( true );
}

void DiceWidget::setMayRollOff( void )
{
    setMayRoll( false );
}

void DiceWidget::toggleMayRoll( void )
{
    setMayRoll( !mayRoll );
}

bool DiceWidget::getMayChange() const
{
    return mayChange;
}

void DiceWidget::setMayChange(bool newState)
{
    emit mayChangeChanged(mayChange);
    mayChange = newState;
    update();
}

void DiceWidget::drawDot( QPainter& painter, QPoint dotPos )
{
    painter.drawEllipse( QPointF( dotPos ) * dotRadius, dotRadius, dotRadius );
}

void DiceWidget::drawCenterDot( QPainter& painter )
{
    drawDot( painter, QPoint() );
}

void DiceWidget::drawTopLeftBottomRightDots( QPainter& painter )
{
    drawDot( painter, QPoint( -4, -4 ) );
    drawDot( painter, QPoint(  4,  4 ) );
}

void DiceWidget::drawBottomLeftTopRightDots(QPainter& painter)
{
    drawDot( painter, QPoint( -4,  4 ) );
    drawDot( painter, QPoint(  4, -4 ) );
}

void DiceWidget::drawSideMiddleDots( QPainter& painter )
{
    drawDot( painter, QPoint( -4,  0 ) );
    drawDot( painter, QPoint(  4,  0 ) );
}

void DiceWidget::rollDice( void )
{
    if( !mayRoll ) return;
    int newValue = rand() % 6 + 1;
    if( value != newValue )
    {
        value = newValue;
        update();
        emit valueChanged( newValue );
    }
    emit diceRolled( newValue );
}

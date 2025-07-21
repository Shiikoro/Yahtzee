#ifndef DICEWIDGET_H
#define DICEWIDGET_H

#include <QtWidgets>

class DiceWidget : public QWidget
{
    Q_OBJECT
private:
    int           value;
    bool          clickStarted;
    int           diceSize;
    double        dotRadius;
    QRect         diceRect;
    bool          mayRoll;
    bool          mayChange;

    void          drawDot( QPainter& painter, QPoint dotPos );
    void          drawCenterDot( QPainter& painter );
    void          drawTopLeftBottomRightDots( QPainter& painter );
    void          drawBottomLeftTopRightDots( QPainter& painter );
    void          drawSideMiddleDots( QPainter& painter );

public:
    explicit      DiceWidget( QWidget* parent = nullptr );
    virtual QSize sizeHint( void ) const override;

    int           getValue( void ) const;
    bool          getMayRoll( void ) const;
    bool          getMayChange() const;



protected:
    virtual void  resizeEvent( QResizeEvent* event ) override;
    virtual void  paintEvent( QPaintEvent* event ) override;
    virtual void  mousePressEvent( QMouseEvent* event ) override;
    virtual void  mouseReleaseEvent( QMouseEvent *event ) override;

public slots:
    void          setMayChange(bool newState);
    void          rollDice( void );
    void          setMayRoll( bool newState );
    void          setMayRollOn( void );
    void          setMayRollOff( void );
    void          toggleMayRoll( void );

signals:
    void          valueChanged( int newValue );
    void          diceRolled( int newValue );
    void          clicked( void );
    void          mayRollChanged( bool newState );
    void          mayChangeChanged(bool newState);
};

#endif // DICEWIDGET_H

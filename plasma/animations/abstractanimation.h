/*
 *   Copyright 2009 Mehmet Ali Akmanalp <makmanalp@wpi.edu>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

/**
 * @file This file contains the classes for AbstractAnimation, which is the
 * abstract base class for all animations.
 */

#ifndef PLASMA_ABSTRACTANIMATION_H
#define PLASMA_ABSTRACTANIMATION_H

#include <QAbstractAnimation>
#include <QEasingCurve>
#include <QGraphicsWidget>
#include <QObject>

#include <plasma/plasma.h>
#include <plasma/plasma_export.h>


namespace Plasma
{

class AbstractAnimationPrivate;

/**
 * @brief Abstract base class for AnimationGroup and Animation.
 * @since 4.4
 */
class PLASMA_EXPORT AbstractAnimation : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QEasingCurve::Type easingCurveType READ easingCurveType WRITE setEasingCurveType)
    Q_PROPERTY(AnimationDirection direction READ direction WRITE setDirection)
    Q_PROPERTY(qreal distance READ distance WRITE setDistance)
    Q_PROPERTY(bool isVisible READ isVisible WRITE setVisible)

public:

    AbstractAnimation();
    virtual ~AbstractAnimation() = 0;

    /**
     * Set the widget on which the animation is to be performed.
     * @arg receiver The QGraphicsWidget to be animated.
     */
    virtual void setWidgetToAnimate(QGraphicsWidget* receiver);

    /**
     * The widget that the animation will be performed upon
     */
    QGraphicsWidget *widgetToAnimate();

    /**
     * Take an AbstractAnimation and turn it into a
     * QAbstractAnimation.
     */
    virtual QAbstractAnimation* toQAbstractAnimation(QObject* parent) = 0;

    /**
     * Set the animation easing curve type
     */
    void setEasingCurveType(QEasingCurve::Type easingCurve);

    /**
     * Get the animation easing curve type
     */
    QEasingCurve::Type easingCurveType() const;

    /**
     * Set the animation direction
     * @arg direction animation direction
     */
    void setDirection(AnimationDirection direction);

    /**
     * Get the animation direction
     */
    AnimationDirection direction() const;

    /**
     * Set the animation distance
     * @distance animation distance
     */
    void setDistance(qreal distance);

    /**
     * Get the animation distance
     */
    qreal distance() const;

    /**
     * set the animation visibility
     * @arg isVisible animation visibility
     */
    void setVisible(bool isVisible);

    /**
     * get the animation visibility
     */
    bool isVisible() const;

public slots:

    /**
     * Start the animation.
     */
    virtual void start() = 0;

protected:

private:
    AbstractAnimationPrivate *d;
};

} //namespace Plasma

#endif
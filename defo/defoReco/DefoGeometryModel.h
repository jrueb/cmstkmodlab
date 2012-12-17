#ifndef DEFOGEOMETRYMODEL_H
#define DEFOGEOMETRYMODEL_H

#include <QObject>

class DefoGeometryModel : public QObject
{
    Q_OBJECT
public:
  explicit DefoGeometryModel(QObject *parent = 0);

  void setAngle1(double v);
  void setAngle2(double v);
  void setDistance(double v);
  void setHeight(double v);

  double getAngle1() const { return angle1_; }
  double getAngle2() const { return angle2_; }
  double getDistance() const { return distance_; }
  double getHeight() const { return height_; }

  void write(const QString& filename);
  void read(const QString& filename);

public slots:

protected:

  double angle1_;
  double angle2_;
  double distance_;
  double height_;

signals:

  void geometryChanged();
};

#endif // DEFOGEOMETRYMODEL_H

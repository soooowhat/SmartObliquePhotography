#pragma once
#include <QSlider>

class QFloatSlider : public QSlider
{
	Q_OBJECT

public:
	QFloatSlider(QWidget* pParent = NULL);

	void setRange(float Min, float Max);
	void setMinimum(float Min);
	float minimum() const;
	void setMaximum(float Max);
	float maximum() const;
	float value() const;

public slots:
	void setValue(int value);
	void setValue(float Value, bool BlockSignals = false);

private slots:

signals:
	void valueChanged(float Value);

private:
	float m_Multiplier;
};

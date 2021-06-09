﻿#pragma once
#include "ui_DisplayOptionsSettingDialog.h"
#include "QTInterface.h"
#include <qdialog.h>
#include "FloatSlider.h"

namespace hivePointClouds::command
{
	struct SRegionGrowingSetting
	{
		enum class EColorMode
		{
			Mean,
			Median,
		};
		float GroundHeight = 446.2f;
		bool bColorFlag = true;
		bool bGroundFlag = true;
		bool bNormalFlag = false;
		EColorMode ColorMode = EColorMode::Mean;
		float SearchSize = 0.5f;
		float ColorThreshold = 5.0f;
	};
}

namespace hiveQTInterface
{
	class CInteractionManager;

	class CDisplayOptionsSettingDialog : public QDialog
	{
		Q_OBJECT

	public:
		CDisplayOptionsSettingDialog(QWidget* vParent)
			: QDialog(vParent),
			m_pUi(std::make_shared<Ui::CDisplayOptionsSettingDialog>())
		{
			m_pUi->setupUi(this);

			this->setWindowFlag(Qt::WindowType::WindowContextHelpButtonHint, false);
			
			m_pUi->ColorTest->setCheckState(m_GrowingSetting.bColorFlag ? Qt::Checked : Qt::Unchecked);
			m_pUi->GroundTest->setCheckState(m_GrowingSetting.bGroundFlag ? Qt::Checked : Qt::Unchecked);
			m_pUi->NormalTest->setCheckState(m_GrowingSetting.bNormalFlag ? Qt::Checked : Qt::Unchecked);
			if (m_GrowingSetting.ColorMode == hivePointClouds::command::SRegionGrowingSetting::EColorMode::Mean)
				m_pUi->AverageButton->setChecked(true);
			else
				m_pUi->MedianButton->setChecked(true);

			//输入
			float MinSize = 0.1f;
			float MaxSize = 2.0f;
			m_pUi->SizeBox->setMinimum(MinSize);
			m_pUi->SizeBox->setMaximum(MaxSize);
			m_pUi->SizeBox->setSingleStep(0.2f);
			m_pUi->SizeBox->setValue(m_GrowingSetting.SearchSize);
			QObject::connect(m_pUi->SizeBox, SIGNAL(valueChanged(double)), this, SLOT(onActionInputSize()));

			//滑条
			m_SearchSizeSlider = std::make_shared<QFloatSlider>(this);
			m_SearchSizeSlider->setObjectName(QString::fromUtf8("SearchSize"));
			m_SearchSizeSlider->setGeometry(QRect(20, 153, 160, 22));
			m_SearchSizeSlider->setOrientation(Qt::Horizontal);
			m_SearchSizeSlider->setMinimum(MinSize);
			m_SearchSizeSlider->setMaximum(MaxSize);
			m_SearchSizeSlider->setValue(m_GrowingSetting.SearchSize);
			QObject::connect(m_SearchSizeSlider.get(), SIGNAL(valueChanged(int)), this, SLOT(onActionChangeSize()));

			//输入
			float MinThreshold = 1.0f;
			float MaxThreshold = 20.0f;
			m_pUi->ThresholdBox->setMinimum(MinThreshold);
			m_pUi->ThresholdBox->setMaximum(MaxThreshold);
			m_pUi->ThresholdBox->setSingleStep(2.0f);
			m_pUi->ThresholdBox->setValue(m_GrowingSetting.ColorThreshold);
			QObject::connect(m_pUi->ThresholdBox, SIGNAL(valueChanged(double)), this, SLOT(onActionInputThreshold()));

			//滑条
			m_ColorThresholdSlider = std::make_shared<QFloatSlider>(this);
			m_ColorThresholdSlider->setObjectName(QString::fromUtf8("ColorThreshold"));
			m_ColorThresholdSlider->setGeometry(QRect(20, 213, 160, 22));
			m_ColorThresholdSlider->setOrientation(Qt::Horizontal);
			m_ColorThresholdSlider->setMinimum(MinThreshold);
			m_ColorThresholdSlider->setMaximum(MaxThreshold);
			m_ColorThresholdSlider->setValue(m_GrowingSetting.ColorThreshold);
			QObject::connect(m_ColorThresholdSlider.get(), SIGNAL(valueChanged(int)), this, SLOT(onActionChangeThreshold()));

			QObject::connect(m_pUi->ColorTest, SIGNAL(stateChanged(int)), this, SLOT(onActionColorTest()));
			QObject::connect(m_pUi->GroundTest, SIGNAL(stateChanged(int)), this, SLOT(onActionGroundTest()));
			QObject::connect(m_pUi->NormalTest, SIGNAL(stateChanged(int)), this, SLOT(onActionNormalTest()));

			QObject::connect(m_pUi->AverageButton, SIGNAL(clicked()), this, SLOT(onActionAverageMode()));
			QObject::connect(m_pUi->MedianButton, SIGNAL(clicked()), this, SLOT(onActionMedianMode()));
		}
		
	private:
		std::shared_ptr<Ui::CDisplayOptionsSettingDialog> m_pUi = nullptr;

		std::shared_ptr<QFloatSlider> m_SearchSizeSlider;
		std::shared_ptr<QFloatSlider> m_ColorThresholdSlider;

		static hivePointClouds::command::SRegionGrowingSetting m_GrowingSetting;

	public slots:
		void onActionColorTest();
		void onActionGroundTest();
		void onActionNormalTest();
		void onActionAverageMode();
		void onActionMedianMode();
		void onActionChangeSize();
		void onActionChangeThreshold();
		void onActionInputSize();
		void onActionInputThreshold();

		friend class CInteractionManager;
	};
}
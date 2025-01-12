﻿#pragma once

#include "ui_DisplayOptionsSettingDialog.h"
#include <qdialog.h>
#include "SceneReconstructionInterface.h"

namespace hiveObliquePhotography::QTInterface
{
	class CDisplayOptionsSettingDialog : public QDialog
	{
		Q_OBJECT

	public:
		CDisplayOptionsSettingDialog(QWidget* vParent, CDisplayOptionsSettingDialog*& vThisInParent)
			: QDialog(vParent), m_ThisInParent(vThisInParent),
			m_pUi(new Ui::CDisplayOptionsSettingDialog)
		{
			this->setAttribute(Qt::WA_DeleteOnClose);

			m_pUi->setupUi(this);
			this->setWindowFlag(Qt::WindowType::WindowContextHelpButtonHint, false);

			m_pUi->ColorFeatureCheckBox->setChecked(m_ColorStatus);
			m_pUi->NormalFeatureCheckBox->setChecked(m_NormalStatus);

			char* FileName = const_cast<char*>("./Config/NormalFeature/PointCloudRetouchConfig.xml");
			std::ifstream out(FileName);
			if (!out.is_open())
				return;
			out.close();

			WCHAR buf[256];
			memset(buf, 0, sizeof(buf));
			MultiByteToWideChar(CP_ACP, 0, FileName, strlen(FileName) + 1, buf, sizeof(buf) / sizeof(buf[0]));
			CopyFile(buf, L"./PointCloudRetouchConfig.xml", false);
			
			QObject::connect(m_pUi->ColorFeatureCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onActionColorFeatureCheckBox()));
			QObject::connect(m_pUi->NormalFeatureCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onActionNormalFeatureCheckBox()));

			QObject::connect(m_pUi->OctreeDepth, SIGNAL(valueChanged(int)), this, SLOT(onActionOctreeDepth()));
			QObject::connect(m_pUi->ResolutionX, SIGNAL(valueChanged(int)), this, SLOT(onActionResolutionXSpinBox()));
			QObject::connect(m_pUi->ResolutionY, SIGNAL(valueChanged(int)), this, SLOT(onActionResolutionYSpinBox()));
			QObject::connect(m_pUi->SurfelRadius, SIGNAL(valueChanged(double)), this, SLOT(onActionSurfelRadius()));
			QObject::connect(m_pUi->NumSample, SIGNAL(valueChanged(int)), this, SLOT(onActionNumSample()));
			QObject::connect(m_pUi->SearchRadius, SIGNAL(valueChanged(double)), this, SLOT(onActionSearchRadius()));
			QObject::connect(m_pUi->DistanceThreshold, SIGNAL(valueChanged(double)), this, SLOT(onActionDistanceThreshold()));
			QObject::connect(m_pUi->WeightCoefficent, SIGNAL(valueChanged(double)), this, SLOT(onActionWeightCoefficient()));

			QObject::connect(m_pUi->OKButton, SIGNAL(clicked()), this, SLOT(onActionOK()));

			if (__initSceneReconstructionConfig() == false)
			{
				_HIVE_OUTPUT_WARNING("Failed to read config of SceneReconstruction.");
			}
		}
		
		~CDisplayOptionsSettingDialog()
		{
			//通知主窗口该窗口已经关闭
			m_ThisInParent = nullptr;
		}

	private:
		bool __initSceneReconstructionConfig();
		void __refreshConfigPannel();

		static bool m_ColorStatus;
		static bool m_NormalStatus;
		static SceneReconstruction::CSceneReconstructionConfig* m_pSceneReconstructionConfig;
		static int m_OctreeDepth;
		static std::tuple<int, int> m_Resolution;
		static double m_SurfelRadius;
		static int m_NumSample;
		static double m_SearchRadius;
		static double m_DistanceThreshold;
		static double m_WeightCoefficient;

	private slots:
		void onActionColorFeatureCheckBox();
		void onActionNormalFeatureCheckBox();

		void onActionOctreeDepth();
		void onActionResolutionXSpinBox();
		void onActionResolutionYSpinBox();
		void onActionSurfelRadius();
		void onActionNumSample();
		void onActionSearchRadius();
		void onActionDistanceThreshold();
		void onActionWeightCoefficient();

		void onActionOK();

	private:
		CDisplayOptionsSettingDialog*& m_ThisInParent;
		Ui::CDisplayOptionsSettingDialog* m_pUi = nullptr;
	};
}
// 2025 John Warren

#include "SIK.h"

#include "SIKRuntime.h"

FSIKSettings USIK::Initialize(FSIKDefaults Defaults, FSIKCalibrationData Calibration)
{
	const float LowerArmRatio = 0.48f;

	FSIKSettings Settings;

	Settings.ArmLength = (Calibration.Height / 2.0f) - Calibration.UpperArmsDistance;
	Settings.LowerArmLength = Settings.ArmLength * LowerArmRatio;
	Settings.UpperArmLength = Settings.ArmLength * (1.0f - LowerArmRatio);
	Settings.HeadHandAngleLimit = Defaults.HeadHandAngleLimit;
	Settings.HeadHandAngleLimitDot = FMath::Cos(FMath::DegreesToRadians(Settings.HeadHandAngleLimit));

	Settings.ClavicleOffset = Defaults.ClavicleOffset;
	Settings.DistinctShoulderRotationLimit = Defaults.DistinctShoulderRotationLimit;
	Settings.DistinctShoulderRotationMultiplier = Defaults.DistinctShoulderRotationMultiplier;
	Settings.ElbowBaseOffsetAngle = Defaults.ElbowBaseOffsetAngle;
	Settings.ElbowHandsRotSpeed = Defaults.ElbowHandsRotSpeed;
	Settings.ElbowRotFromHandRotAlpha = Defaults.ElbowRotFromHandRotAlpha;
	Settings.ElbowYDistanceStart = Defaults.ElbowYDistanceStart;
	Settings.ElbowYWeight = Defaults.ElbowYWeight;
	Settings.UpperArmsDistance = Calibration.UpperArmsDistance;
	Settings.OkSpanAngle = Defaults.OkSpanAngle;
	Settings.BaseCharOffset = Defaults.BaseCharOffset;
	Settings.BodyInterSpeed = Defaults.BodyInterSpeed;
	Settings.LocalHandOffset = Defaults.LocalHandOffset;
	Settings.LocalHandRotationOffset = Defaults.LocalHandRotationOffset;
	Settings.ShoulderHeadHandAlpha = Defaults.ShoulderHeadHandAlpha;
	return Settings;
}

FTransform USIK::AddLocalOffset(const FTransform Transform, const FVector Offset)
{
	FTransform Inversed = Transform.Inverse();
	Inversed.AddToTranslation(Offset);
	return Inversed.Inverse();
}

FTransform USIK::RotatePointAroundPivot(FTransform Point, FTransform Pivot, FRotator Delta)
{
	FTransform PointInPivotSpace = Point * Pivot.Inverse();
	FTransform RotatedInPivotSpace = PointInPivotSpace * FTransform(Delta, FVector::ZeroVector, FVector::OneVector);
	return RotatedInPivotSpace * Pivot;
}

float USIK::SafeguardAngle(float Last, float Current, float Threshold)
{
	return (FMath::Abs(Last - Current) > Threshold) ? (Last) : (Current);
}

float USIK::CosineRule(float Adjacent1, float Adjacent2, float Opposite)
{
	return FMath::RadiansToDegrees(
		FMath::Acos(
		((Adjacent1 * Adjacent1) + (Adjacent2 * Adjacent2) - (Opposite * Opposite)) /
			(Adjacent1 * Adjacent2 * 2.f)
		)
	);
}

FRotator USIK::FindBetweenNormals(const FVector& A, const FVector& B)
{
	return FQuat::FindBetweenNormals(A, B).Rotator();
}

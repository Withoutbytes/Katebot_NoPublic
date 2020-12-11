#ifndef _TRIGGERBOT_H_
#define _TRIGGERBOT_H_

#include "..\main.h"

class Triggerbot
{
public:
	void Start()
	{
		try {
			while (true)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(1));

				if (!cfg->triggerbotEnabled)
					continue;

				if (!csgo->IsInGame())
					continue;

				if (LocalEntity.IsDead())
					continue;

				//printf(xorstr("Accuracy: %.4f\n").crypt_get(), LocalEntity.GetAccuracyPenalty());
				if (input_system::IsButtonDown(input_system::vktobc(cfg->keys.triggerbot_hold)))
				{
					bool shoot = false;

					switch (cfg->triggerbot.fovBased)
					{
					case true:
						shoot = FovTriggerbot();
						break;
					case false:
						shoot = InCross() != -1;
						break;
					}

					if (shoot) {
						Sleep(cfg->triggerbot.delay);
						csgo->PressAttackKey();
						Sleep(10);
						csgo->ReleaseAttackKey();
						Sleep(5);
					}
				}
			}
		}
		catch (...) {
			mem->debuglog(__FILE__);
		}
	}

private:
	float GetDistance(Vector v1, Vector v2)
	{
		return (float)sqrt((double)((v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y) + (v1.z - v2.z) * (v1.z - v2.z)));
	}

	int InCross()
	{
		int IncrossIndex = mem->Read<int>(LocalEntity.GetPointer() + ofs->m_iCrossHairID);
		if (IncrossIndex > 0 && IncrossIndex < 65) {
			for (int i = 0; i < csgo->GetMaxClients(); i++) {
				if (EntityList[i].GetIndex() == IncrossIndex)
					if (EntityList[i].GetHealth() > 0 && !csgo->IsInMyTeam(EntityList[i].GetPointer()))
						return i;
			}
		}
		return -1;
	}

	bool FovTriggerbot()
	{
		Vector current, PunchAngles;
		csgo->GetViewAngles(current);

		PunchAngles = LocalEntity.GetPunchAngles();
		PunchAngles.z = 0.0f;
		current -= PunchAngles * 2;

		for (int i = 0; i < csgo->GetMaxClients(); i++) {
			if (csgo->IsInMyTeam(EntityList[i].GetPointer()))
				continue;

			if (!EntityList[i].IsValid())
				continue;

			/*if (InBone(current, i, 8, 2))
				return true;

			if (InBone(current, i, 6, 2))
				return true;

			if (InBone(current, i, 4, 2))
				return true;*/

			if (InBone(current, LocalEntity.GetEyePosition(), EntityList[i].GetBonePosition(8)))
				return true;

			if (InBone(current, LocalEntity.GetEyePosition(), EntityList[i].GetBonePosition(6)))
				return true;

			if (InBone(current, LocalEntity.GetEyePosition(), EntityList[i].GetBonePosition(4)))
				return true;

		}
		return false;
	}

	/*bool TraceRayTrigger() {
		for (int x = 0; x < csgo->GlobalVars().maxClients; x++)
		{
			Vector HeadBone = EntityList[x].GetBonePosition(8);

			Vector BottomHitboxHead = Vector(HeadBone.x - 2.54f, HeadBone.y - 4.145f, HeadBone.z - 7.f);
			Vector TopHitboxHead = Vector(HeadBone.x + 2.54f, HeadBone.y + 4.145f, HeadBone.z + 3.f);

			Vector hBone = EntityList[x].GetBonePosition(3);

			Vector BottomHitboxBody = Vector(hBone.x - 7.f, hBone.y - 5.5f, hBone.z - 25.f);
			Vector TopHitboxBody = Vector(hBone.x + 7.f, hBone.y + 5.5f, hBone.z + 15.f);

			Vector ViewAngles;
			csgo->GetViewAngles(ViewAngles);

			Vector viewDirection = Cray::AngleToDirection(ViewAngles);
			Cray ViewRay(LocalEntity.GetEyePosition(), viewDirection);

			float distance = 0.f;

			if (ViewRay.Trace(BottomHitboxHead, TopHitboxHead, distance))
				return true;
			else
				return false;
		}
		return false;
	}*/

	void MakeVector(Vector angle, Vector& vector)
	{
		float pitch = float(angle[0] * M_PI / 180);
		float yaw = float(angle[1] * M_PI / 180);
		float tmp = float(cos(pitch));
		vector.x = float(-tmp * -cos(yaw));
		vector.y = float(sin(yaw) * tmp);
		vector.z = float(-sin(pitch));
	}

	bool InBone(Vector angle, Vector src, Vector dst)
	{
		Vector ang, aim;
		ang = CalcAngle(src, dst);
		MakeVector(angle, aim);
		MakeVector(ang, ang);

		float mag = FastSQRT(pow(aim.x, 2) + pow(aim.y, 2) + pow(aim.z, 2));
		float u_dot_v = aim.DotProduct(ang);

		return (RAD2DEG(acos(u_dot_v / (pow(mag, 2)))) < cfg->triggerbot.fov);
	}

	Vector CalcAngle(Vector src, Vector dst)
	{
		Vector ret;
		Vector vDelta = src - dst;
		float fHyp = FastSQRT((vDelta.x * vDelta.x) + (vDelta.y * vDelta.y));

		ret.x = (atanf(vDelta.z / fHyp)) * (180.0f / (float)M_PI);
		ret.y = (atanf(vDelta.y / vDelta.x)) * (180.0f / (float)M_PI);

		if (vDelta.x >= 0.0f)
			ret.y += 180.0f;

		return ret;
	}

	//bool InBone(Vector& current, int m_target, int bone, float fov)
	//{
	//	auto direction = EntityList[m_target].GetBonePosition(bone) - LocalEntity.GetEyePosition();
	//	VectorNormalize(direction);

	//	auto target = VectorAngles(direction);
	//	ClampAngles(target);

	//	Vector v_dist = LocalEntity.GetOrigin() - EntityList[m_target].GetOrigin();
	//	auto distance = FastSQRT((v_dist.x * v_dist.x) + (v_dist.y * v_dist.y) + (v_dist.z * v_dist.z));

	//	// modify 20.f as needed, depending on bone, random atm
	//	if (CanHit(current, target, distance, fov))
	//	{
	//		return true;
	//	}

	//	return false;
	//}

	bool CanHit(Vector current, Vector target, float distance, float max)
	{
		auto delta = target - current;
		ClampAngles(delta);

		auto xdist = sinf(DEG2RAD(delta.Length())) * distance / 2.f;

		return xdist <= max;

		return false;
	}

	Vector VectorAngles(const Vector& direction)
	{
		float pitch, yaw;

		if (!direction.x && !direction.y)
		{
			pitch = direction.z > 0.f ? 270.f : 90.f;
			yaw = 0.f;
		}
		else
		{
			pitch = RAD2DEG(atan2f(-direction.z, direction.LengthXY()));
			pitch += pitch < 0.f ? 360.f : 0.f;

			yaw = RAD2DEG(atan2f(direction.y, direction.x));
			yaw += yaw < 0.f ? 360.f : 0.f;
		}

		return Vector(pitch, yaw, 0.f);
	}

	float VectorNormalize(Vector& vector)
	{
		auto length = vector.Length();
		vector = length ? vector / length : Vector();

		return length;
	}

	float AngleNormalize(float angle)
	{
		return isfinite(angle) ? remainder(angle, 360.f) : 0.f;
	}

	void ClampAngles(Vector& angles)
	{
		angles.x = std::max(-89.f, std::min(89.f, AngleNormalize(angles.x)));
		angles.y = AngleNormalize(angles.y);
		angles.z = 0.f;
	}

};

#endif 
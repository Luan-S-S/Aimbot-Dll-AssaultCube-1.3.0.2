#include "Windows.h"
#include "math.h"

constexpr float PI = 3.1415927f; // Constante Pi usada nos cálculos de ângulos

struct Vector2
{
public:
	float X; // Coordenada X 2D
	float Y; // Coordenada Y 2D
};

struct Vector3
{
public:
	float X; // Coordenada X 3D
	float Y; // Coordenada Y 3D
	float Z; // Coordenada Z 3D
};

struct Rifles //Tamanho 0x14 = 20 decimal
{
public:
	int MTP_57_Assault_Rifle;       // Identificador de rifle MTP-57
	int A_ARD10_Submachine_Gun;     // Identificador de submetralhadora ARD10
	int Precision_Tech_AD_81_Sniper_Rifle; // Identificador de sniper AD-81
	int V_19_Combat_Shotgun;        // Identificador de shotgun V-19
	int TMP_MeA_Carbine;            // Identificador de carabina TMP-MeA
};

struct ViewMatrix // tamanho 0x40 = 64 decimal
{
private:
	float m11, m12, m13, m14,  // Componentes da matriz 4x4
		m21, m22, m23, m24,
		m31, m32, m33, m34,
		m41, m42, m43, m44;

	/*public: // Placeholder para futuras implementações de ESP (mostrar jogadores na tela)
		bool WorldToScreen(const Vector3& worldPosPlayer, const int& width, const int& height, Vector2* screenPosPlayer)
		{
			if (TransformW(worldPosPlayer) < 0.0) // Verifica se o ponto está atrás da câmera
			{
				return false;
			}
			// Aqui normalmente se continuaria a projeção do ponto para a tela
		}
	*/
private:
	float TransformX(const Vector3& worldPos) const
	{
		return (m11 * worldPos.X) + (m21 * worldPos.Y) + (m31 * worldPos.Z) + m41; // Transformação X
	}

	float TransformY(const Vector3& worldPos) const
	{
		return (m12 * worldPos.X) + (m22 * worldPos.Y) + (m32 * worldPos.Z) + m42; // Transformação Y
	}

	float TransformW(const Vector3& worldPos) const
	{
		return (m14 * worldPos.X) + (m24 * worldPos.Y) + (m34 * worldPos.Z) + m44; // Coordenada W (profundidade)
	}
};

struct Instructions
{
private:	                      //|    Inicia    |  Tamanho  |   Termina 
	char pad_0x00000000[0xC2EC3]; //| (0x00000000) | (0xC2EC3) | (0x000C2EC2) _pad
	BYTE Recoil[5];				  //| (0x000C2EC3) |   (0x5)   | (0x000C2EC7) -> bytes de recoil
	char pad_0x004C2EC8[0x4354];  //| (0x000C2EC8) |  (0x4354) | (0x000C721B) _pad
	BYTE SpeedShot[2];            //| (0x000C721C) |    (0x2)  | (0x000C721D) -> bytes para tiro rápido
	char pad_0x004C721E[0x1D1];   //| (0x000C721E) |   (0x1D1) | (0x000C73EE) _pad
	BYTE AmmoDecrement[2];        //| (0x000C73EF) |    (0x2)  | (0x000C73F0) -> decremento de munição
	char pad_0x004C73F1[0x19E1];  //| (0x000C73F1) |  (0x19E1) | (0x000C8DD1) _pad
	BYTE RepulsionX[5];           //| (0x000C8DD2) |    (0x5)  | (0x000C8DD6) -> repulsão X
	char pad_0x004C8DD7[0x24];    //| (0x000C8DD7) |   (0x24)  | (0x000C8DFA) _pad
	BYTE RepulsionY[5];           //| (0x000C8DFB) |    (0x5)  | (0x000C8DFF) -> repulsão Y
	char pad_0x004C8E00[0x1E9];   //| (0x000C8E00) |   (0x1E9) | (0x000C8FE8) _pad
	BYTE BagDecrement[2];         //| (0x000C8FE9) |   (0x2)   | (0x000C8FEA) -> decremento de mochila

	// Bytes originais dos cheats (para restaurar)
	static constexpr BYTE RECOIL_ORIGINAL_BYTES[5] = { 0xF3, 0x0F, 0x11, 0x56, 0x38 };
	static constexpr BYTE REPULSION_X_ORIGINAL_BYTES[5] = { 0xF3, 0x0F, 0x11, 0x40, 0x10 };
	static constexpr BYTE REPULSION_Y_ORIGINAL_BYTES[5] = { 0xF3, 0x0F, 0x11, 0x40, 0x14 };
	static constexpr BYTE SPEED_SHOT_ORIGINAL_BYTES[2] = { 0x2B, 0x0A };
	static constexpr BYTE AMMO_DECREMENT_ORIGINAL_BYTES[2] = { 0xFF, 0x08 };
	static constexpr BYTE BAG_DECREMENT_ORIGINAL_BYTES[2] = { 0x29, 0x30 };

	// Bytes modificados para aplicar cheats
	static constexpr BYTE RECOIL_MODIFIED_BYTES[5] = { 0x90, 0x90, 0x90, 0x90, 0x90 };
	static constexpr BYTE REPULSION_X_MODIFIED_BYTES[5] = { 0x90, 0x90, 0x90, 0x90, 0x90 };
	static constexpr BYTE REPULSION_Y_MODIFIED_BYTES[5] = { 0x90, 0x90, 0x90, 0x90, 0x90 };
	static constexpr BYTE SPEED_SHOT_MODIFIED_BYTES[2] = { 0x90, 0x90 };
	static constexpr BYTE AMMO_DECREMENT_MODIFIED_BYTES[2] = { 0x90, 0x90 };
	static constexpr BYTE BAG_DECREMENT_MODIFIED_BYTES[2] = { 0x90, 0x90 };


public:
	void NoRecoil(bool activate)
	{
		ControlActivation(&Recoil, RECOIL_ORIGINAL_BYTES, RECOIL_MODIFIED_BYTES, 5, activate); // Ativa/desativa No Recoil
	}

	void RapidFire(bool activate)
	{
		ControlActivation(&SpeedShot, SPEED_SHOT_ORIGINAL_BYTES, SPEED_SHOT_MODIFIED_BYTES, 2, activate); // Ativa/desativa Rapid Fire
	}

	void InfiniteAmmo(bool activate)
	{
		ControlActivation(&AmmoDecrement, AMMO_DECREMENT_ORIGINAL_BYTES, AMMO_DECREMENT_MODIFIED_BYTES, 2, activate); // Ativa/desativa munição infinita
	}

	void NoRepulsion(bool activate)
	{
		ControlActivation(&RepulsionX, REPULSION_X_ORIGINAL_BYTES, REPULSION_X_MODIFIED_BYTES, 5, activate); // Remove repulsão X
		ControlActivation(&RepulsionY, REPULSION_Y_ORIGINAL_BYTES, REPULSION_Y_MODIFIED_BYTES, 5, activate); // Remove repulsão Y
	}

	void InfiniteBag(bool activate)
	{
		ControlActivation(&BagDecrement, BAG_DECREMENT_ORIGINAL_BYTES, BAG_DECREMENT_MODIFIED_BYTES, 2, activate); // Ativa/desativa mochila infinita
	}

	void AllCheats(bool noRecoil, bool rapidFIre, bool infiniteAmmo, bool infiniteBag, bool noRepulsion)
	{
		// Ativa todos os cheats de uma vez
		NoRecoil(noRecoil);
		RapidFire(rapidFIre);
		InfiniteAmmo(infiniteAmmo);
		InfiniteBag(infiniteBag);
		NoRepulsion(noRepulsion);
	}

private:
	// Modifica os bytes de memória diretamente
	void ModifyMemoryBytes(void* address, const BYTE newBytes[], size_t numOfBytes)
	{
		DWORD oldProc;
		VirtualProtect(address, numOfBytes, PAGE_EXECUTE_READWRITE, &oldProc); // Permite escrita na memória
		MoveMemory(address, newBytes, numOfBytes); // Copia os novos bytes
		VirtualProtect(address, numOfBytes, oldProc, &oldProc); // Restaura proteção original
	}

	// Ativa ou desativa cheat dependendo do parâmetro 'activate'
	void ControlActivation(void* address, const BYTE originalBytes[], const BYTE modifiedBytes[], size_t numOfBytes, bool activate)
	{
		if (activate)
		{
			ModifyMemoryBytes(address, modifiedBytes, numOfBytes);
			return;
		}
		ModifyMemoryBytes(address, originalBytes, numOfBytes);
	}

};

// --- Player Class ---
class Player
{
private:					  //|  Inicia  |  Tamanho | Termina 
	char pad_0x0000[0x4];	  //| (0x0000) |   (0x4)  | (0x0003) _pad
	Vector3 HeadPosition;	  //| (0x0004) |   (0xC)  | (0x000F) -> posição da cabeça
	char pad_0x0010[0x18];	  //| (0x0010) |  (0x18)  | (0x0027) _pad
	Vector3 FeetPosition;	  //| (0x0028) |   (0xC)  | (0x0033) -> posição dos pés
	Vector2 VAngles;		  //| (0x0034) |   (0x8)  | (0x003B) -> ângulos da visão (yaw, pitch)
	char pad_0x003C[0xB0];	  //| (0x003C) |  (0xB0)  | (0x00EB) _pad
	int Health;               //| (0x00EC) |   (0x4)  | (0x00EF) -> vida do jogador (0-100)
	int Vest;			      //| (0x00F0) |   (0x4)  | (0x00F3) -> colete/armadura
	char pad_0x00F4[0x14];    //| (0x00F4) |  (0x14)  | (0x0107) _pad                                                                            
	int PistolBag;		      //| (0x0108) |   (0x4)  | (0x010B) -> quantidade de pistolas
	Rifles PrimaryBag;	      //| (0x010C) |  (0x14)  | (0x011F) -> rifles disponíveis na mochila
	char pad_0x0120[0xC];     //| (0x0120) |   (0xC)  | (0x012B) _pad
	int PistolAmmunition;     //| (0x012C) |   (0x4)  | (0x012F) -> munição pistola
	Rifles PrimaryAmmunition; //| (0x0130) |  (0x14)  | (0x0143) -> munição rifles
	int Grenade;              //| (0x0144) |   (0x4)  | (0x0147) -> granadas
	char pad_0x0148[0xBD];    //| (0x0148) |  (0xBD)  | (0x0204) _pad
	char Name[15];            //| (0x0205) |   (0xF)  | (0x0213) -> nome do jogador
	char pad_0x0214[0xF8];    //| (0x0214) |  (0xF8)  | (0x030B) _pad
	char Team;                //| (0x030C) |   (0x1)  | (0x030C) -> time do jogador (para modos de equipe)

public:

	void Aimbot(Player*& target, const BYTE& gameMode)
	{
		// Mira automaticamente no alvo caso seja válido
		if (IsValidTarget(target, gameMode))
		{
			MoveAim(target->FeetPosition); // Ajusta mira para a posição dos pés
		}
	}

	void ESP(bool activate)
	{
		// Placeholder para futuras implementações de ESP (mostrar jogadores na tela)
	}

	void SetHealth(int newHealth)
	{
		this->Health = newHealth; // Altera a vida do jogador
	}

	Player* SelectAimbotTarget(Player*& currentClosest, Player*& candidate, const BYTE& gameMode) const
	{
		// Seleciona o melhor alvo para o aimbot
		bool currentValid = IsValidTarget(currentClosest, gameMode);
		bool candidateValid = IsValidTarget(candidate, gameMode);

		if ((!currentValid) && (!candidateValid)) return nullptr; // Nenhum alvo válido
		if (!currentValid) return candidate; // Somente candidato é válido
		if (!candidateValid) return currentClosest; // Somente atual é válido

		return GetCloserTarget(currentClosest, candidate); // Retorna o mais próximo
	}

	bool IsAlive() const
	{
		// Verifica se o jogador está vivo (Health > 0 e <= 100)
		return this->Health > 0 && this->Health <= 100;
	}


private:
	Player* GetCloserTarget(Player*& currentClosest, Player*& candidate) const
	{
		// Compara distâncias para decidir o alvo mais próximo
		float distCurrent = this->GetDistanceTo(currentClosest->FeetPosition);
		float distCandidate = this->GetDistanceTo(candidate->FeetPosition);

		return (distCandidate < distCurrent) ? candidate : currentClosest;
	}

	float GetDistanceTo(const Vector3& position) const
	{
		// Calcula distância 3D entre este jogador e uma posição
		float dx = this->FeetPosition.X - position.X;
		float dy = this->FeetPosition.Y - position.Y;
		float dz = this->FeetPosition.Z - position.Z;

		return sqrtf(dx * dx + dy * dy + dz * dz);
	}

	Vector2 ComputeViewAnglesTo(const Vector3& targetPosition) const
	{
		// Calcula ângulos (Yaw, Pitch) para mirar no alvo
		Vector2 viewAngles;

		float deltaX = targetPosition.X - this->FeetPosition.X;
		float deltaY = targetPosition.Y - this->FeetPosition.Y;
		float deltaZ = targetPosition.Z - this->FeetPosition.Z;

		float horizontalDistance = sqrtf(deltaX * deltaX + deltaY * deltaY);

		viewAngles.X = atan2f(deltaY, deltaX) * 180.0f / PI + 90.0f; // Yaw (horizontal)
		viewAngles.Y = atan2f(deltaZ, horizontalDistance) * 180.0f / PI; // Pitch (vertical)

		return viewAngles;
	}

	void MoveAim(const Vector3& targetPosition)
	{
		// Aplica os ângulos de mira calculados
		Vector2 desiredAngles = ComputeViewAnglesTo(targetPosition);

		this->VAngles.X = desiredAngles.X;
		this->VAngles.Y = desiredAngles.Y;
	}

	bool IsEnemyTo(Player*& otherPlayer) const
	{
		// Checa se outro jogador é inimigo (diferente time)
		return this->Team != otherPlayer->Team;
	}

	bool IsTeamGameMode(const BYTE& mode) const
	{
		// Determina se o modo de jogo é de equipe (Team)
		return (mode == 7 || mode == 20 || mode == 21);
	}

	bool IsValidTarget(Player*& target, const BYTE& gameMode) const
	{
		// Checa se o alvo é válido para o aimbot
		if (target == nullptr || !target->IsAlive())
		{
			return false; // Alvo inexistente ou morto
		}

		if (IsTeamGameMode(gameMode) && !this->IsEnemyTo(target))
		{
			return false; // Ignora aliados em modo team
		}

		return true;
	};
};

// --- AssaultCube Class ---
class AssaultCube
{
public:                           //|    Inicia    |  Tamanho  |   Termina 
	Instructions Instr;           //| (0x00000000) | (0xC8FEB) | (0x000C8FEA) -> manipulador de cheats
private:
	char pad_0x004C8FEB[0xB4FE5]; //| (0x000C8FEB) | (0xB4FE5) | (0x0017DFCF) _pad
public:
	ViewMatrix VMatrix;           //| (0x0017DFD0) |   (0x40)  | (0x0017E00F) -> matriz de visão do jogo
private:
	char pad_0x0057E010[0xCBE8];  //| (0x0017E010) |  (0xCBE8) | (0x0058ABF7) _pad
public:
	BYTE GameMode;                //| (0x0018ABF8) |   (0x1)   | (0x0018ABF8) -> modo de jogo atual            
private:
	char pad_0x0058ABF9[7];       //| (0x0018ABF9) |   (0x7)   | (0x0018ABFF) _pad
public:
	Player* LocalPlayer;          //| (0x0018AC00) |   (0x4)   | (0x0018AC03) -> ponteiro para jogador local
private:
	char pad_0x0018AC04[0x72D4];  //| (0x0018AC04) |  (0x72D4) | (0x00191ED7) _pad
public:
	int Width;                    //| (0x00191ED8) |   (0x4)   | (0x00191EDB) -> largura da tela
private:
	char pad_0x00191EDC[0x8];     //| (0x00191EDC) |   (0x8)   | (0x00191EE3) _pad
public:
	int Height;                   //| (0x00191EE4) |   (0x4)   | (0x00191EE7) -> altura da tela
private:
	char pad_0x00191EE8[0x10];    //| (0x00191EE8) |   (0x10)  | (0x00191EF7) _pad
public:
	int ResolutionX;              //| (0x00191EF8) |   (0x4)   | (0x00191EFB) -> resolução X
	int ResolutionY;              //| (0x00191EFC) |   (0x4)   | (0x00191EFF) -> resolução Y
private:
	char pad_0x00191F00[0xCC];    //| (0x00191F00) |   (0xCC)  | (0x00191FCB) _pad
public:
	Player** EntityList;          //| (0x00191FCC) |   (0x4)   | (0x00191FCF) -> lista de jogadores no mapa
private:
	char pad_0x00191FD0[0x4];     //| (0x00191FD0) |   (0x4)   | (0x00191FD3) _pad
public:
	int NumPlayers;               //| (0x00191FD4) |   (0x4)   | (0x00191FD3) -> número total de jogadores
};
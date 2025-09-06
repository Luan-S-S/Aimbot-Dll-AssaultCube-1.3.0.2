#pragma once
class Hook
{
private:
	Hook() = delete;  // Desativa construtor padr�o (n�o permite instanciar a classe)
	~Hook() = delete; // Desativa destrutor (n�o permite instanciar a classe)

	// Fun��o interna que cria um JMP (jump) para desviar a execu��o
	static void SetJmp(void* orig, void* dst, DWORD offset)
	{
		DWORD addr = (DWORD)orig + offset;           // Calcula endere�o base + offset
		DWORD distance = ((DWORD)dst - ((DWORD)orig + 5)); // Calcula dist�ncia relativa do JMP (dst - prox instru��o)
		*(BYTE*)addr = 0xE9;                        // 0xE9 = opcode JMP rel32
		*(DWORD*)(addr + 0x1) = distance;           // Escreve deslocamento do JMP
	}

public:
	// Cria um detour (hook) em uma fun��o
	template <typename TypeFunc>
	static bool CreateDeTour(void* hkFuncAddr, const char* hkFuncName, const char* moduleName, TypeFunc myFunc, DWORD numBytes, TypeFunc& pTrp)
	{
		// Valida os par�metros: n�o deve passar tudo nulo nem tudo preenchido
		if ((hkFuncAddr == nullptr && hkFuncName == nullptr && moduleName == nullptr) ||
			(hkFuncAddr != nullptr && hkFuncName != nullptr && moduleName != nullptr))
			return false;

		// Valida que nome da fun��o e m�dulo s�o passados juntos, ou ambos nulos
		if ((hkFuncName != nullptr && moduleName == nullptr) ||
			(moduleName != nullptr && hkFuncName == nullptr))
			return false;

		TypeFunc hkTargetFunc = (TypeFunc)hkFuncAddr; // Inicializa ponteiro da fun��o alvo

		// Se endere�o direto n�o foi passado, obt�m fun��o via GetProcAddress
		if (hkTargetFunc == nullptr)
			hkTargetFunc = (TypeFunc)GetProcAddress(GetModuleHandleA(moduleName), hkFuncName);

		DWORD oldProtect[2]; // Armazena permiss�es antigas de mem�ria

		// Valida se fun��o existe, tamanho m�nimo para hook � 5 bytes, e altera prote��o para RWX
		if (hkTargetFunc == nullptr || numBytes < 5 || (!VirtualProtect(hkTargetFunc, numBytes, PAGE_EXECUTE_READWRITE, &oldProtect[0])))
			return false;

		// Aloca mem�ria para trampoline (copia da fun��o original)
		pTrp = (TypeFunc)VirtualAlloc(0, numBytes + 0x5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

		if (pTrp == nullptr) // Falha ao alocar
			return false;

		memcpy(pTrp, hkTargetFunc, numBytes); // Copia bytes originais para o trampoline

		// Adiciona JMP no final do trampoline de volta para a fun��o original
		SetJmp(pTrp, hkTargetFunc, numBytes);

		memset(hkTargetFunc, 0x90, numBytes); // NOP (0x90) nos bytes originais da fun��o para limpar

		// Coloca JMP da fun��o original para nossa fun��o hook
		SetJmp(hkTargetFunc, myFunc, 0);

		// Restaura permiss�es originais e retorna sucesso
		return VirtualProtect(hkTargetFunc, numBytes, oldProtect[0], &oldProtect[1]);
	}
};

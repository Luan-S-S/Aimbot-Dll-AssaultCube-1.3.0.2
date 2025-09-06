#pragma once
class Hook
{
private:
	Hook() = delete;  // Desativa construtor padrão (não permite instanciar a classe)
	~Hook() = delete; // Desativa destrutor (não permite instanciar a classe)

	// Função interna que cria um JMP (jump) para desviar a execução
	static void SetJmp(void* orig, void* dst, DWORD offset)
	{
		DWORD addr = (DWORD)orig + offset;           // Calcula endereço base + offset
		DWORD distance = ((DWORD)dst - ((DWORD)orig + 5)); // Calcula distância relativa do JMP (dst - prox instrução)
		*(BYTE*)addr = 0xE9;                        // 0xE9 = opcode JMP rel32
		*(DWORD*)(addr + 0x1) = distance;           // Escreve deslocamento do JMP
	}

public:
	// Cria um detour (hook) em uma função
	template <typename TypeFunc>
	static bool CreateDeTour(void* hkFuncAddr, const char* hkFuncName, const char* moduleName, TypeFunc myFunc, DWORD numBytes, TypeFunc& pTrp)
	{
		// Valida os parâmetros: não deve passar tudo nulo nem tudo preenchido
		if ((hkFuncAddr == nullptr && hkFuncName == nullptr && moduleName == nullptr) ||
			(hkFuncAddr != nullptr && hkFuncName != nullptr && moduleName != nullptr))
			return false;

		// Valida que nome da função e módulo são passados juntos, ou ambos nulos
		if ((hkFuncName != nullptr && moduleName == nullptr) ||
			(moduleName != nullptr && hkFuncName == nullptr))
			return false;

		TypeFunc hkTargetFunc = (TypeFunc)hkFuncAddr; // Inicializa ponteiro da função alvo

		// Se endereço direto não foi passado, obtém função via GetProcAddress
		if (hkTargetFunc == nullptr)
			hkTargetFunc = (TypeFunc)GetProcAddress(GetModuleHandleA(moduleName), hkFuncName);

		DWORD oldProtect[2]; // Armazena permissões antigas de memória

		// Valida se função existe, tamanho mínimo para hook é 5 bytes, e altera proteção para RWX
		if (hkTargetFunc == nullptr || numBytes < 5 || (!VirtualProtect(hkTargetFunc, numBytes, PAGE_EXECUTE_READWRITE, &oldProtect[0])))
			return false;

		// Aloca memória para trampoline (copia da função original)
		pTrp = (TypeFunc)VirtualAlloc(0, numBytes + 0x5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

		if (pTrp == nullptr) // Falha ao alocar
			return false;

		memcpy(pTrp, hkTargetFunc, numBytes); // Copia bytes originais para o trampoline

		// Adiciona JMP no final do trampoline de volta para a função original
		SetJmp(pTrp, hkTargetFunc, numBytes);

		memset(hkTargetFunc, 0x90, numBytes); // NOP (0x90) nos bytes originais da função para limpar

		// Coloca JMP da função original para nossa função hook
		SetJmp(hkTargetFunc, myFunc, 0);

		// Restaura permissões originais e retorna sucesso
		return VirtualProtect(hkTargetFunc, numBytes, oldProtect[0], &oldProtect[1]);
	}
};

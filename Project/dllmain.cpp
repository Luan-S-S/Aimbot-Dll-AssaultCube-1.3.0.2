#include "pch.h"
#include "Structures.h"
#include "Hook.h"
#define AC_BASE_ADDRESS 0x00400000

// Ponteiro para estrutura principal do jogo, mapeando mem�ria do AssaultCube
AssaultCube* ac = (AssaultCube*)0x00400000;

bool activationFlag = false; // Flag que ativa/desativa os cheats
HANDLE hThread;             // Handle da thread principal do cheat
int keyAimbot = VK_RBUTTON; // Bot�o usado para ativar o aimbot (bot�o direito do mouse)

// Tipo da fun��o original WglSwapBuffers
typedef void (WINAPI* TypeWglSwapBuffers) (HDC hDc);
TypeWglSwapBuffers wglSwapBuffers; // Ponteiro para a fun��o original

// Retorna true se a tecla especificada estiver pressionada
bool IsKeyPressed(int key)
{
    return (GetAsyncKeyState(key) & 0x8000);
}

// Alterna o valor de uma flag (true -> false / false -> true)
void ToggleFlag(bool& flag)
{
    flag = !flag;
}

// Ativa/desativa cheats ao pressionar uma tecla, com som de beep
bool ToggleCheatOnKeyPress(int keyActivation, bool& flag)
{
    if (IsKeyPressed(keyActivation))
    {
        Beep(750, 500); // Feedback sonoro
        ToggleFlag(flag);
        return true;
    }
    return false;
}

// Aplica os cheats principais (NoRecoil, RapidFire, InfiniteBag, NoRepulsion)
void ApplyCheats(const bool& activationFlag)
{
    ac->Instr.AllCheats(activationFlag, activationFlag, false, activationFlag, activationFlag);
}

// Mant�m a vida do jogador local sempre cheia
void InfiniteLife()
{
    ac->LocalPlayer->SetHealth(100);
}

// Fun��o que executa o aimbot
void RunAimbot()
{
    static Player* targetPlayer = nullptr; // Armazena o alvo atual

    if (IsKeyPressed(keyAimbot)) // Verifica se tecla de mira autom�tica est� pressionada
    {
        if (targetPlayer == nullptr) // Se n�o houver alvo, seleciona um
        {
            for (int i = 0; i < ac->NumPlayers; i++)
            {
                targetPlayer = ac->LocalPlayer->SelectAimbotTarget(targetPlayer, ac->EntityList[i], ac->GameMode);
            }
        }
        // Mira automaticamente no alvo selecionado
        ac->LocalPlayer->Aimbot(targetPlayer, ac->GameMode);
    }

    // Reseta o alvo se a tecla foi solta ou o alvo morreu
    if (targetPlayer != nullptr && ((!IsKeyPressed(keyAimbot) || (!targetPlayer->IsAlive()))))
    {
        targetPlayer = nullptr;
    }
}

// Fun��o hookada que substitui WglSwapBuffers
void WINAPI HookedWglSwapBuffers(_In_ HDC hDc)
{
    if (activationFlag)
    {
        InfiniteLife(); // Mant�m vida infinita
        RunAimbot();    // Executa aimbot
    }

    return wglSwapBuffers(hDc); // Chama fun��o original
}

// Thread principal do cheat
DWORD WINAPI CheatMainThread(LPVOID)
{
    // Cria o hook para WglSwapBuffers
    if (!Hook::CreateDeTour(NULL, "wglSwapBuffers", "opengl32.dll", HookedWglSwapBuffers, 5, wglSwapBuffers))
    {
        MessageBoxA(NULL, "Hook Falhou", "", MB_OK);
    }

    // Loop infinito para monitorar entrada do usu�rio
    while (true)
    {
        if (ToggleCheatOnKeyPress(VK_INSERT, activationFlag)) // Ativa/desativa cheats com INSERT
        {
            ApplyCheats(activationFlag); // Aplica cheats
        }
        Sleep(100); 
    }
}

// Entrada da DLL
BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        // Cria a thread principal do cheat quando a DLL � carregada
        hThread = CreateThread(NULL, 0, CheatMainThread, NULL, 0, NULL);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        // Fecha handle da thread quando DLL � descarregada ou thread � criada/destro�da
        CloseHandle(hThread);
        break;
    }
    return TRUE;
}


<!-- Header -->
<div align="center" style="padding: 24px 12px;">
    <h1 style="margin: 0 0 8px; font-size: 2.2rem;">Cheat Interno (DLL Inject) – AssaultCube-1.3.0.2</h1>
    <p style="margin: 0; font-size: 1.05rem; opacity: .85;">
        Demonstração educacional de um cheat interno injetado por DLL, com hook em <code>wglSwapBuffers</code>,
        uso de <em>Struct Padding</em> e funcionalidades como Aimbot, No Recoil e Rapid Fire.
    </p>
    <br />
    <p>
        <img alt="status" src="https://img.shields.io/badge/status-demo-blue">
        <img alt="language" src="https://img.shields.io/badge/lang-C%2B%2B-%2300599C">
        <img alt="opengl" src="https://img.shields.io/badge/OpenGL-hooking-success">
        <img alt="dll" src="https://img.shields.io/badge/DLL-injection-orange">
    </p>
</div>

<hr style="border: 0; border-top: 1px solid #e5e7eb; margin: 24px 0;" />

<!-- Sumário -->
<div>
    <h2 id="sobre" style="margin-top: 0.5em;">📌 Sobre o projeto</h2>
    <p>
        Este repositório demonstra o funcionamento de um <strong>cheat interno em C++</strong>, injetado por meio
        de uma <strong>DLL</strong>. O projeto realiza <strong>hooking</strong> em pontos críticos do ciclo de renderização
        do jogo para implementar suas funcionalidades, utilizando também a técnica de <strong>Struct Padding</strong> para alinhar
        e manipular estruturas de memória de forma segura/prevista.
    </p>
    <p>
        Ao injetar a DLL, o hook é ativado diretamente na função <code>wglSwapBuffers</code> — responsável pela etapa de
        apresentação de frame no <strong>OpenGL</strong>. A partir disso, os recursos do cheat podem ser habilitados via atalhos
        de teclado/mouse.
    </p>
</div>

<!-- Funcionalidades -->
<div>
    <h2 id="funcionalidades">✨ Funcionalidades</h2>
    <ul style="line-height: 1.85;">
        <li><strong>Ativação geral:</strong> tecla <code>Insert</code></li>
        <li><strong>Aimbot:</strong> botão direito do mouse</li>
        <li><strong>No Recoil:</strong> remoção/redução de recuo</li>
        <li><strong>Rapid Fire:</strong> disparo rápido</li>
        <li><strong>Infinite Ammo:</strong> munição infinita</li>
        <li><strong>Infinite Bag:</strong> capacidade/mochila infinita</li>
        <li><strong>No Repulsion:</strong> sem empurrão/knockback</li>
    </ul>
</div>

<!-- Atalhos -->
<div>
    <h2 id="atalhos">⌨️ Atalhos</h2>
    <table style="border-collapse: collapse; width: 100%; max-width: 760px;">
        <thead>
            <tr>
                <th style="text-align: left; border-bottom: 1px solid #e5e7eb; padding: 8px;">Ação</th>
                <th style="text-align: left; border-bottom: 1px solid #e5e7eb; padding: 8px;">Atalho</th>
            </tr>
        </thead>
        <tbody>
            <tr>
                <td style="padding: 8px; border-bottom: 1px dashed #eee;">Ativar/Desativar Cheat</td>
                <td style="padding: 8px; border-bottom: 1px dashed #eee;"><code>Insert</code></td>
            </tr>
            <tr>
                <td style="padding: 8px; border-bottom: 1px dashed #eee;">Aimbot</td>
                <td style="padding: 8px; border-bottom: 1px dashed #eee;">Botão direito do mouse</td>
            </tr>
        </tbody>
    </table>
</div>

<!-- Como funciona -->
<div>
    <h2 id="como-funciona">🧠 Como funciona</h2>
    <ol style="line-height: 1.85;">
        <li><strong>Injeção da DLL</strong>: a DLL é carregada no processo do jogo.</li>
        <li><strong>Hook em <code>wglSwapBuffers</code></strong>: a função de apresentação de frame do OpenGL é interceptada.</li>
        <li><strong>Execução dos recursos</strong>: durante o ciclo de renderização, as rotinas (aimbot, recoil, etc.) são aplicadas.</li>
        <li><strong>Struct Padding</strong>: estruturas são alinhadas/padronizadas para leitura/escrita previsível na memória alvo.</li>
    </ol>
</div>

<!-- Destaques técnicos -->
<div>
    <h2 id="tecnicos">🛠️ Destaques técnicos</h2>
    <ul style="line-height: 1.85;">
        <li>Hooking em API gráfica (<code>wglSwapBuffers</code>) para sincronizar com o ciclo de render.</li>
        <li>Manipulação de estruturas com <em>Struct Padding</em> para compatibilidade binária.</li>
        <li>Arquitetura baseada em <strong>DLL</strong> com inicialização automática no <em>attach</em>.</li>
    </ul>
</div>

<!-- Requisitos -->
<div>
    <h2 id="requisitos">📦 Requisitos (exemplo)</h2>
    <ul style="line-height: 1.85;">
        <li>Windows</li>
        <li>AssaultCube-1.3.0.2</li>
        <li>Permissões para injeção no processo (ambiente de testes)</li>
    </ul>
</div>

<!-- Aviso legal -->
<div>
    <h2 id="aviso-legal">⚠️ Aviso legal</h2>
    <p style="opacity: .9;">
        Este projeto tem caráter <strong>educacional e de pesquisa</strong>, destinado ao estudo de técnicas de injeção, hooking
        e manipulação de memória. <strong>Não</strong> incentive o uso em ambientes online/competitivos ou em violação de Termos de Uso.
        O autor e contribuidores não se responsabilizam por mau uso.
    </p>
</div>

<!-- Rodapé -->
<hr style="border: 0; border-top: 1px solid #e5e7eb; margin: 24px 0;" />
<p align="center" style="opacity: .8;">
    Se este projeto te ajudou, considere deixar uma ⭐ no repositório.
</p>


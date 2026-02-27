
# WiFiManager
## Versão DE DESENVOLVIMENTO

Gerenciador de conexão WiFi Espressif ESPx com portal de configuração da web de fallback

⚠️ Esta documentação está desatualizada, veja as notas abaixo

![Release](https://img.shields.io/github/v/release/tzapu/WiFiManager?include_prereleases)

[![Build CI Status](https://github.com/tzapu/WiFiManager/actions/workflows/compile_library.yml/badge.svg)](https://github.com/tzapu/WiFiManager/actions/workflows/compile_library.yml)

[![Build CI Status Examples](https://github.com/tzapu/WiFiManager/actions/workflows/compile_examples.yaml/badge.svg)](https://github.com/tzapu/WiFiManager/actions/workflows/compile_examples.yaml)

[![arduino-library-badge](https://www.ardu-badge.com/badge/WiFiManager.svg?)](https://www.ardu-badge.com/WiFiManager)

[![Build with PlatformIO](https://img.shields.io/badge/PlatformIO-Library-orange?)](https://platformio.org/lib/show/567/WiFiManager/installation)

[![ESP8266](https://img.shields.io/badge/ESP-8266-000000.svg?longCache=true&style=flat&colorA=CC101F)](https://www.espressif.com/en/products/socs/esp8266)

[![ESP32](https://img.shields.io/badge/ESP-32-000000.svg?longCache=true&style=flat&colorA=CC101F)](https://www.espressif.com/en/products/socs/esp32)
[![ESP32](https://img.shields.io/badge/ESP-32S2-000000.svg?longCache=true&style=flat&colorA=CC101F)](https://www.espressif.com/en/products/socs/esp32-s2)
[![ESP32](https://img.shields.io/badge/ESP-32C3-000000.svg?longCache=true&style=flat&colorA=CC101F)](https://www.espressif.com/en/products/socs/esp32-c3)

Suporte de Membro para Membro / Bate-papo

 [![Junte-se ao chat em https://gitter.im/tablatronix/WiFiManager](https://badges.gitter.im/tablatronix/WiFiManager.svg)](https://gitter.im/tablatronix/WiFiManager?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)
 
[![Discord](https://img.shields.io/badge/Discord-WiFiManager-%237289da.svg?logo=discord)](https://discord.gg/nS5WGkaQH5)
O portal de configuração é do tipo cativo, portanto, em vários dispositivos, ele apresentará o diálogo de configuração assim que você se conectar ao ponto de acesso criado.

**Funciona com a plataforma Arduino ESP8266**

[https://github.com/esp8266/Arduino](https://github.com/esp8266/Arduino)

**Funciona com a plataforma Arduino ESP32**

[https://github.com/espressif/arduino-esp32](https://github.com/espressif/arduino-esp32)

### Problemas conhecidos
* A documentação precisa ser atualizada, consulte [https://github.com/tzapu/WiFiManager/issues/500](https://github.com/tzapu/WiFiManager/issues/500)
-------

## Conteúdo
 - [Como funciona](#como-funciona)
 - [Lista de Desejos](#lista-de-desejos)
 - [Começo rápido](#começo-rápido)
   - [Instalando](#instalando)
     - [Arduino - Através do Library Manager](#instalar-através-do-library-manager)
     - [Arduino - Do Github](#check-out-do-github)
     - [PlatformIO](#instale-usando-platformio)
   - [Usando](#usando)
 - [Documentação](#documentação)
   - [Senha do ponto de acesso](#senha-do-ponto-de-acesso)
    - [Retornos de chamada](#retornos-de-chamada)
    - [Tempo limite do portal de configuração](#tempo-limite-do-portal-de-configuração)
    - [Configuração sob demanda](#configuração-sob-demanda)
    - [Parâmetros personalizados](#parâmetros-personalizados)
    - [Configuração de IP personalizada](#configuração-de-ip-personalizada)
    - [Filtrar redes de baixa qualidade](#filtrar-redes)
    - [Saída de depuração](#depuração)
  - [Solução de problemas](#solução-de-problemas)
  - [Lançamentos](#lançamentos)
  - [Contribuidores](#contribuições-e-agradecimentos)


## Como funciona
- Quando o seu ESP é inicializado, ele o configura no modo Estação e tenta se conectar a um ponto de acesso salvo anteriormente
- se isso não for bem-sucedido (ou nenhuma rede anterior salva), ele move o ESP para o modo de ponto de acesso e ativa um DNS e um servidor Web (ip padrão 192.168.4.1)
- usando qualquer dispositivo habilitado para wi-fi com um navegador (computador, telefone, tablet), conecte-se ao ponto de acesso recém-criado
- por causa do Captive Portal e do servidor DNS, você obterá um pop-up do tipo 'Entrar na rede' ou obterá qualquer domínio que tentar acessar redirecionado para o portal de configuração
- escolha um dos pontos de acesso verificados, digite a senha, clique em salvar
- O ESP tentará se conectar. Se for bem-sucedido, ele devolve o controle ao seu aplicativo. Caso contrário, reconecte ao AP e reconfigure.
- Existem opções para alterar esse comportamento ou iniciar manualmente o configportal e o webportal de forma independente, bem como executá-los no modo sem bloqueio.

## Como parece
![ESP8266 WiFi Captive Portal Homepage](http://i.imgur.com/YPvW9eql.png) ![ESP8266 WiFi Captive Portal Configuration](http://i.imgur.com/oicWJ4gl.png)

## Lista de Desejos
- [x] remover a dependência da biblioteca EEPROM
- [x] mover strings HTML para PROGMEM
- [x] limpeza e simplificação do código (embora isso esteja em andamento)
- [x] se o tempo limite for definido, estenda-o quando uma página for buscada no modo AP
- [x] adicionar capacidade de configurar mais parâmetros do que ssid/senha
- [x] talvez permita a configuração do ip do ESP após a reinicialização
- [x] adicionar ao Arduino Library Manager
- [x] adicionar a PlatformIO
- [ ] adicionar vários conjuntos de credenciais de rede
- [x] permite que os usuários personalizem o CSS
- [ ] reescrever a documentação para simplificar, com base em cenários/objetivos

### Desenvolvimento
- [x] suporte ESP32
- [x] contam com a conexão automática integrada do SDK mais do que forçar uma conexão
- [x] adicionar modo sem bloqueio
- [x] personalização fácil de strings
- [x] suporte a nome de host
- [x] corrigir vários bugs e soluções alternativas para problemas do esp SDK
- [x] itens adicionais da página de informações
- [x] última exibição de status / motivo da falha
- [x] menu personalizável
- [x] página de parâmetros personalizados separados
- [x] portal on-demand
- [x] refatoração completa do código para segmentar funções
- [x] ícones de varredura wiif ou exibição de porcentagem
- [x] inverter classe para modo escuro
- [x] mais tokens de modelo
- [x] programa para todas as strings
- [ ] novos retornos de chamada
- [ ] novos textos explicativos/filtros
- [ ] instância de servidor web compartilhada
- [x] suporte esp idf/sdk mais recente
- [x] wm agora não é persistente, não irá apagar ou alterar a configuração do esp armazenado no esp8266
- [x] toneladas de saída / níveis de depuração
- [ ] desativar captiveportal
- [ ] pré-carregar wiifscans, carregamentos de página mais rápidos
- [ ] correções de estabilidade softap quando sta não está conectado


## Começo rápido

### Instalando
Você pode instalar através do Arduino Library Manager ou verificar as alterações mais recentes ou uma versão do github

#### Instalar através do Library Manager
__Atualmente a versão 0.8+ funciona com a versão 2.4.0 ou mais recente do [núcleo ESP8266 para Arduino](https://github.com/esp8266/Arduino)__
  - no Arduino IDE, acesse Sketch/Include Library/Manage Libraries
   ![Gerenciar bibliotecas](http://i.imgur.com/9BkEBkR.png)
   
 - procure por WiFiManager
  ![Pacote WiFiManager](http://i.imgur.com/18yIai8.png)

 - clique em Instalar e comece a [usá-lo](#usando)

#### Check-out do github
__A versão do Github funciona com a versão 2.4.0 ou mais recente do [núcleo ESP8266 para Arduino](https://github.com/esp8266/Arduino)__
- Checkout da biblioteca para a pasta de bibliotecas do Arduino

### Usando
- Inclua no seu esboço
```cpp
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic
```

- Inicialize a biblioteca, em sua função de configuração, adicione OBSERVAÇÃO, se você estiver usando o modo não bloqueador, certifique-se de criar isso no escopo global ou manuseá-lo adequadamente, pois não funcionará se estiver na configuração e usando o modo não bloqueador.
```cpp
WiFiManager wifiManager;
```

- Também na função de configuração, adicione
```cpp
//o primeiro parâmetro é o nome do ponto de acesso, o segundo é a senha
wifiManager.autoConnect("AP-NAME", "AP-PASSWORD");
```
se você quiser apenas um ponto de acesso não seguro
```cpp
wifiManager.autoConnect("AP-NAME");
```
ou se você quiser usar um nome gerado automaticamente de 'ESP' e o ID do chip do esp, use
```cpp
wifiManager.autoConnect();
```

Depois de escrever seu esboço e iniciar o ESP, ele tentará se conectar ao WiFi. Se falhar, inicia no modo Access Point.
Enquanto estiver no modo AP, conecte-se a ele e abra um navegador para o IP do gateway, padrão 192.168.4.1, configure o wifi, salve e ele deve reiniciar e conectar.

Veja também [exemplos](https://github.com/tzapu/WiFiManager/tree/master/examples).

#### Install Using PlatformIO

[PlatformIO](https://platformio.org/) é um ecossistema emergente para desenvolvimento de IoT e
é uma alternativa ao uso do Arduino IDE. Instale `WiFiManager`
usando o platformio [gerenciador de biblioteca](https://docs.platformio.org/en/latest/librarymanager/index.html#librarymanager) em seu editor,
ou usando a [PlatformIO Core CLI](https://docs.platformio.org/en/latest/core/index.html),
ou adicionando-o ao seu `platformio.ini` como mostrado abaixo (abordagem recomendada).

A maneira mais simples é abrir o arquivo `platformio.ini` na raiz do seu projeto e `WifiManager` para a chave env `lib_deps` 
comum de nível superior da seguinte forma:

```
[env]
lib_deps =
	WiFiManager
```


```
[env]
lib_deps =
	https://github.com/tzapu/WiFiManager.git
```

## Documentação

#### Senha protege o ponto de acesso de configuração
Você pode e deve proteger com senha o ponto de acesso de configuração. Simplesmente adicione a senha como um segundo parâmetro para `autoConnect`.
Uma senha curta parece ter resultados imprevisíveis, portanto, use uma com cerca de 8 caracteres ou mais.
As diretrizes são que uma senha wi-fi deve consistir de 8 a 63 caracteres codificados em ASCII no intervalo de 32 a 126 (decimal)
```cpp
wifiManager.autoConnect("AutoConnectAP", "password")
```

#### Chamadas de retorno
##### Entre no modo de configuração
Use isso se precisar fazer algo quando seu dispositivo entrar no modo de configuração em uma tentativa de conexão sem fio com falha.
Antes de `autoConnect()`
```cpp
wifiManager.setAPCallback(configModeCallback);
```
`configModeCallback` declaração e exemplo
```cpp
void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());

  Serial.println(myWiFiManager->getConfigPortalSSID());
}
```

##### Salvar configurações
Isso é chamado quando parâmetros personalizados foram definidos **E** uma conexão foi estabelecida. Use-o para definir um sinalizador, assim, quando toda a configuração terminar, você poderá salvar os parâmetros extras em algum lugar.


SE VOCÊ PRECISA SALVAR PARÂMETROS MESMO EM WIFI FAIL OU VAZIO, você deve definir `setBreakAfterConfig` como true, ou então saveConfigCallback não será chamado.

```C++
//se for definido, ele sairá após a configuração, mesmo se a conexão não for bem-sucedida.
    void          setBreakAfterConfig(boolean shouldBreak);
```

Veja [Exemplo de AutoConnectWithFSParameters](https://github.com/tzapu/WiFiManager/tree/master/examples/Parameters/SPIFFS/AutoConnectWithFSParameters).
```cpp
wifiManager.setSaveConfigCallback(saveConfigCallback);
```
`saveConfigCallback` declaração e exemplo
```cpp
//sinalizador para salvar dados
bool shouldSaveConfig = false;

//retorno de chamada nos notificando da necessidade de salvar a configuração
void saveConfigCallback () {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}
```

#### Tempo Limite do Portal de Configuração
Se você precisar definir um tempo limite para que o ESP não trave esperando ser configurado, por exemplo, após uma queda de energia, você pode adicionar
```cpp
wifiManager.setConfigPortalTimeout(180);
```
que aguardará 3 minutos (180 segundos). Quando o tempo passar, a função autoConnect retornará, independentemente do resultado.
Verifique a conexão e se ainda não estiver estabelecida, faça o que for necessário (em alguns módulos eu os reinicio para tentar novamente, em outros eu entro em sono profundo)

#### Portal de configuração sob demanda
Se você preferir iniciar o portal de configuração sob demanda em vez de automaticamente em uma tentativa de conexão com falha, isso é para você.

Em vez de chamar `autoConnect()`, que faz toda a configuração do portal de configuração de conexão e failover para você, você precisa usar `startConfigPortal()`. __Não use AMBOS.__

Examplo de uso
```cpp
void loop() {
  // o portal de configuração é solicitado?
  if ( digitalRead(TRIGGER_PIN) == LOW ) {
    WiFiManager wifiManager;
    wifiManager.startConfigPortal("OnDemandAP");
    Serial.println("connected...yeey :)");
  }
}
```
Veja o exemplo para uma versão mais complexa. [OnDemandConfigPortal](https://github.com/tzapu/WiFiManager/tree/master/examples/OnDemand/OnDemandConfigPortal)

#### Saindo do Portal de Configuração
Normalmente, uma vez inserido, o portal de configuração continuará em loop até que as credenciais WiFi sejam inseridas com sucesso ou um tempo limite seja atingido.
Se preferir sair sem se conectar a uma rede Wi-Fi, diga que vai colocar o ESP no modo AP e pressione o botão "Sair"
na página principal.
Se iniciado via `autoConnect` ou `startConfigPortal`, retornará `false (portalAbortResult)`

#### Parâmetros personalizados
Você pode usar WiFiManager para coletar mais parâmetros do que apenas SSID e senha.
Isso pode ser útil para configurar itens como host e porta MQTT, tokens [blynk](http://www.blynk.cc) ou [emoncms](http://emoncms.org), apenas para citar alguns.
**Você é responsável por salvar e carregar esses valores personalizados.** A biblioteca apenas coleta e exibe os dados para sua conveniência.
O cenário de uso seria:
- carregar valores de algum lugar (EEPROM/FS) ou gerar alguns padrões
- adicione os parâmetros personalizados ao WiFiManager usando
```cpp
 // id/nome, placeholder/prompt, padrão, comprimento
 WiFiManagerParameter custom_mqtt_server("server", "mqtt server", mqtt_server, 40);
 wifiManager.addParameter(&custom_mqtt_server);

```
- se a conexão com o AP falhar, o portal de configuração é iniciado e você pode definir/alterar os valores (ou usar o portal de configuração sob demanda)
- uma vez que a configuração é feita e a conexão estabelecida, save config callback() é chamado
- assim que o WiFiManager retornar o controle ao seu aplicativo, leia e salve os novos valores usando o objeto `WiFiManagerParameter`.
```cpp
 mqtt_server = custom_mqtt_server.getValue();
```  
Este recurso é muito mais complicado do que todos os outros, então aqui estão alguns exemplos para mostrar como isso é feito.
Você também deve dar uma olhada em como adicionar HTML personalizado ao seu formulário.

- Salve e carregue parâmetros personalizados no sistema de arquivos no formato json [AutoConnectWithFSParameters](https://github.com/tzapu/WiFiManager/tree/master/examples/Parameters/SPIFFS/AutoConnectWithFSParameters)
- *Salvar e carregar parâmetros personalizados para EEPROM* (ainda não feito)
#### Configuração de IP personalizada
Você pode definir um IP personalizado para AP (ponto de acesso, modo de configuração) e STA (modo de estação, modo de cliente, estado normal do projeto)

##### Configuração de IP do ponto de acesso personalizado
Isso definirá seu portal cativo para um IP específico, caso você precise/deseja tal recurso. Adicione o seguinte trecho antes de `autoConnect()`
```cpp
//definir ip personalizado para portal
wifiManager.setAPStaticIPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));
```

##### Estação personalizada (cliente) Configuração de IP estático
Isso fará com que use a configuração de IP especificada em vez de usar o DHCP no modo de estação.
```cpp
wifiManager.setSTAStaticIPConfig(IPAddress(192,168,0,99), IPAddress(192,168,0,1), IPAddress(255,255,255,0)); // 4º argumento DNS opcional
```
Existem alguns exemplos na pasta de exemplos que mostram como definir um IP estático e até mesmo como configurá-lo através do portal de configuração da web.

NOTA: Você deve preencher o servidor DNS se tiver solicitações HTTP com nomes de host ou tempo de sincronização (NTP). É o mesmo ip do gateway ou um popular (Google DNS: 8.8.8.8).

#### HTML personalizado, CSS, Javascript
Existem várias maneiras de injetar HTML, CSS ou Javascript personalizado no portal de configuração.
As opções são:
- injetar elemento de cabeça personalizado
Você pode usar isso para qualquer bit html no cabeçalho do portal de configuração. Se você adicionar um elemento `<style>`, tenha em mente que ele subrescreve o css incluído, não substitui.
```cpp
wifiManager.setCustomHeadElement("<style>html{filter: invert(100%); -webkit-filter: invert(100%);}</style>");
```
- injetar um bit personalizado de html no formulário de configuração/parâmetro
```cpp
WiFiManagerParameter custom_text("<p>This is just a text paragraph</p>");
wifiManager.addParameter(&custom_text);
```
- injetar um bit personalizado de html em um elemento de formulário de configuração
Basta adicionar o bit que deseja adicionar como o último parâmetro ao construtor de parâmetro personalizado.
```cpp
WiFiManagerParameter custom_mqtt_server("server", "mqtt server", "iot.eclipse", 40, " readonly");
wifiManager.addParameter(&custom_mqtt_server);
```

#### Tema
Você pode personalizar certos elementos do modelo padrão com algumas classes internas
```CPP
wifiManager.setClass("invert"); // tema escuro
wifiManager.setScanDispPerc(true); // exibir porcentagens em vez de gráficos para RSSI
```
Existem classes adicionais no css que você pode usar em seu html personalizado, veja o modelo de exemplo.

#### Filtrar Redes
Você pode filtrar redes com base na qualidade do sinal e mostrar/ocultar redes duplicadas.

- Se você deseja filtrar redes com baixa qualidade de sinal, pode dizer ao WiFiManager para não mostrar redes abaixo de uma % de qualidade arbitrária;
```cpp
wifiManager.setMinimumSignalQuality(10);
```
não mostrará redes abaixo de 10% de qualidade de sinal. Se você omitir o parâmetro, o padrão será 8%;

- Você também pode remover ou mostrar redes duplicadas (o padrão é remover).
Use esta função para mostrar (ou ocultar) todas as redes.
```cpp
wifiManager.setRemoveDuplicateAPs(false);
```

#### Depurar
A depuração é ativada por padrão em `Serial` em versões não estáveis. Para desativar a adição antes de autoConnect/startConfigPortal
```cpp
wifiManager.setDebugOutput(false);
```

Você pode passar em um fluxo personalizado via construtor
```CPP
WiFiManager wifiManager(Serial1);
```

Você pode personalizar o nível de depuração alterando `_debugLevel` na fonte
as opções são:
* DEBUG_ERROR
* DEBUG_NOTIFY
* DEBUG_VERBOSE
* DEBUG_DEV
* DEBUG_MAX

## Solução de problemas
Se você receber erros de compilação, na maioria das vezes, pode ser necessário instalar uma versão mais recente do núcleo ESP8266 para Arduino.

As alterações adicionadas na versão 0.8 devem fazer com que o trunk mais recente funcione sem erros de compilação. Testado até o núcleo ESP8266 2.0.0. **Atualize para a versão 0.8**

Estou tentando manter os releases funcionando com as versões de lançamento do core, para que possam ser instaladas através do gerenciador de placas, mas se você fizer o checkout da versão mais recente diretamente do github, às vezes, a biblioteca só funcionará se você atualizar o core ESP8266 para o mais recente versão porque estou usando alguma função recém-adicionada.

Se você se conectar ao Access Point de configuração criado, mas o portal de configuração não aparecer, basta abrir um navegador e digitar o IP do portal web, por padrão `192.168.4.1`.

Se tentar conectar terminar em um loop infinito, tente adicionar `setConnectTimeout(60)` antes de `autoConnect();`. O parâmetro é o tempo limite para tentar conectar em segundos.

Fico preso no modo ap quando a energia acaba ou o modem reinicia, tente um setConfigPortalTimeout(segundos). Isso fará com que o configportal feche após nenhuma atividade e você poderá reinicializar ou tentar a reconexão em seu código.

## Lançamentos
### 1.0.1

### Visão geral do desenvolvimento

#### Métodos públicos adicionados
`setConfigPortalBlocking`

`setShowStaticFields`

`setCaptivePortalEnable`

`setRestorePersistent`

`setCaptivePortalClientCheck`

`setWebPortalClientCheck`

`startWebPortal`

`stopWebPortal`

`process`

`disconnect`

`erase`

` debugSoftAPConfig`

` debugPlatformInfo`

`setScanDispPerc`

`setHostname`

`setMenu(menu_page_t[])`

`setWiFiAutoReconnect`

` setSTAStaticIPConfig(..,dns)`

`setShowDnsFields`

`getLastConxResult`

`getWLStatusString`

`getModeString`

`getWiFiIsSaved`

`setShowInfoErase`

`setEnableConfigPortal`

`setCountry`

`setClass`

`htmleEtities`


#### WiFiManagerParameter
`WiFiManagerParameter(id,label)`

`WiFiManagerParameter.setValue(value,length)`

`getParameters`

`getParametersCount`


#### Constructores
`WiFiManager(Stream& consolePort)`

#### definir flags
❗️  **Defines não podem ser definidos em esboços do usuário**
`#define WM_MDNS       // usar MDNS`

`#define WM_FIXERASECONFIG // use a correção de flash de apagamento, esp8266 2.4.0`

`#define WM_ERASE_NVS // esp32 erase(true) irá apagar NVS`

`#include <rom/rtc.h> // a página de informações do esp32 mostrará os motivos da última reinicialização se este arquivo estiver incluído`

#### Visão geral das alterações
- Suporte ESP32 (bastante estável)
- refatoração completa de strings `strings_en.h`
- adiciona novos tokens para wifiscan e algumas classes (esquerda, inverter ícones, cor MSG)
- adiciona cores padrão, primárias e especiais do painel de texto explicativo de status
- adiciona toneladas de informações na página de informações e capacidade de apagar
- adiciona ícones de sinal, substitui porcentagem (tem títulos flutuantes)
- adiciona rótulos a todas as entradas (substitui espaços reservados)
- todo o html (e eventualmente todas as strings exceto debug) movido para `strings_en.h`
- adicionado depuração adicional, linhas de depuração compactadas, debuglevels
- persistente desativado e restaurado via de/con-stuctor (usa `setRestorePersistent`)
- deve reter todos os modos de usuário, incluindo AP, não deve sobrescrever ou persistir modos ou configurações de usuário, mesmo STA (`storeSTAmode`) (BUGGY)
- ⚠️ os valores de retorno podem ter mudado dependendo da interrupção do portal ou do tempo limite ( `portalTimeoutResult`,`portalAbortResult`)
- a memória dos parâmetros é alocada automaticamente pelo incremento de `WIFI_MANAGER_MAX_PARAMS(5)` quando excedido, o usuário não precisa mais especificar isso.
- addparameter agora retorna bool e retorna false se o ID do parâmetro não for alphanum [0-9,A-Z,a-z,_]
- IDs de campo de parâmetro permitem que o token {I} use param_n em vez de string, caso alguém queira alterar isso devido a i18n ou problemas de caractere
- fornece `#DEFINE FIXERASECONFIG` para ajudar a lidar com https://github.com/esp8266/Arduino/pull/3635
- relatório do motivo da falha no portal
- definir esp8266 sta hostname, esp32 sta+ap hostname (ID do cliente DHCP)
- passar no fluxo de depuração no construtor WiFiManager(Stream& consolePort)
- você pode forçar os campos ip com showxfields(false) se você definir _disableIpFields=true
- menu/página de parâmetros (configuração) adicionados para separar os parâmetros da página wifi, manipulados automaticamente pelo setMenu
- definir menu raiz personalizado
- desativar o configportal na conexão automática
- parâmetros wm init agora estão protegidos, permitindo classes filhas, exemplo incluído
- wifiscans são pré-cacheados e assíncronos para carregamentos de página mais rápidos, atualização força nova varredura
- adiciona esp32 gettemperature (atualmente comentado, útil apenas para medição relativa)
- 
#### 0.12
- 204 header response removida
- corrigida a incompatibilidade com outras libs usando isnan e outras funções std:: sem namespace

##### 0.11
- reconexão muito mais confiável às redes
- html personalizado em parâmetros personalizados (para parâmetros somente leitura)
- html personalizado no formulário de parâmetro personalizado (como rótulos)
- elemento de cabeçalho personalizado (como CSS personalizado)
- classificar redes com base na qualidade do sinal
- remover redes duplicadas

##### 0.10
- algumas mudanças no css
- correções de bugs e melhorias de velocidade
- adicionou uma alternativa ao waitForConnectResult() para depuração
- mudou `setTimeout(segundos)` para `setConfigPortalTimeout(segundos)`

### Contribuições e agradecimentos
O apoio e a ajuda que recebi da comunidade foram fenomenais. Eu não posso agradecer a vocês o suficiente. Esta é minha primeira tentativa real de desenvolver coisas de código aberto e devo dizer que agora entendo porque as pessoas se dedicam tanto a isso, é por causa de todas as pessoas maravilhosas envolvidas.

__AGRADECIMENTOS__

Os mantenedores esp8266 e esp32 arduino e idf!

[Shawn A aka tablatronix](https://github.com/tablatronix)

[liebman](https://github.com/liebman)

[Evgeny Dontsov](https://github.com/dontsovcmc)

[Chris Marrin](https://github.com/cmarrin)

[bbx10](https://github.com/bbx10)

[kentaylor](https://github.com/kentaylor)

[Maximiliano Duarte](https://github.com/domonetic)

[alltheblinkythings](https://github.com/alltheblinkythings)

[Niklas Wall](https://github.com/niklaswall)

[Jakub Piasecki](https://github.com/zaporylie)

[Peter Allan](https://github.com/alwynallan)

[John Little](https://github.com/j0hnlittle)

[markaswift](https://github.com/markaswift)

[franklinvv](https://github.com/franklinvv)

[Alberto Ricci Bitti](https://github.com/riccibitti)

[SebiPanther](https://github.com/SebiPanther)

[jonathanendersby](https://github.com/jonathanendersby)

[walthercarsten](https://github.com/walthercarsten)

E inúmeros outros

#### Inspiração
 * http://www.esp8266.com/viewtopic.php?f=29&t=2520
 * https://github.com/chriscook8/esp-arduino-apboot
 * https://github.com/esp8266/Arduino/tree/master/libraries/DNSServer/examples/CaptivePortalAdvanced
 * Feito por AlexT https://github.com/tzapu


#include "../../include/Systems/HUDSystem.h"
#include "../../include/Game.h"
#include "../../include/Entity.h"
#include "../../include/Components/PlayerComponent.h"
#include "../../include/Components/HealthComponent.h"
#include <spdlog/spdlog.h>
#include <sstream>
#include <iomanip>

HUDSystem::HUDSystem(const GameContext &ctx)
    : m_Renderer(ctx.renderer), m_WorldWidth(ctx.worldWidth), m_WorldHeight(ctx.worldHeight)
{
}

void HUDSystem::renderRectangle(int x, int y, int width, int height, int r, int g, int b, int a)
{
    SDL_SetRenderDrawColor(m_Renderer, r, g, b, a);
    SDL_FRect rect{static_cast<float>(x), static_cast<float>(y), static_cast<float>(width), static_cast<float>(height)};
    SDL_RenderFillRect(m_Renderer, &rect);
}

void HUDSystem::renderText(const std::string &text, int x, int y)
{
    // Función reservada para futuras mejoras con SDL_ttf
}

void HUDSystem::update(World &world, float dt)
{
    if (!m_Renderer)
        return;

    // Encontrar al jugador (O(1) - acceso directo, sin iteración)
    Entity *playerEntity = world.GetPlayer();

    if (!playerEntity)
        return;

    auto *playerComp = playerEntity->GetComponent<PlayerComponent>();
    auto *healthComp = playerEntity->GetComponent<HealthComponent>();

    if (!playerComp || !healthComp)
        return;

    // ========================================
    // RENDERIZAR INFORMACIÓN EN PANTALLA
    // ========================================

    // Panel HUD (fondo semi-transparente en la esquina superior izquierda)
    const int PANEL_WIDTH = 250; // Ancho del fondo negro
    const int PANEL_HEIGHT = 50; // Alto del fondo negro
    const int PADDING = 15;      // Distancia desde las esquinas

    // Fondo del HUD
    renderRectangle(PADDING, PADDING, PANEL_WIDTH, PANEL_HEIGHT, 0, 0, 0, 180);

    // ========================================
    // BARRA DE SALUD DEL JUGADOR
    // ========================================
    const int BAR_X = PADDING * 2;
    const int BAR_Y = PADDING * 2;
    const int HP_BOX_SIZE = 20; // Tamaño de cada caja de HP
    const int HP_SPACING = 4;   // Espacio entre cajas

    // Mostrar HP como cajas: verde (vida) y gris (perdida)
    // Cada caja representa 1 HP
    for (int i = 0; i < healthComp->maxHp; i++)
    {
        int boxX = BAR_X + (i * (HP_BOX_SIZE + HP_SPACING));

        if (i < healthComp->hp)
        {
            // Verde: HP actual
            renderRectangle(boxX, BAR_Y, HP_BOX_SIZE, HP_BOX_SIZE, 50, 200, 50, 255);
        }
        else
        {
            // Gris oscuro: HP perdido
            renderRectangle(boxX, BAR_Y, HP_BOX_SIZE, HP_BOX_SIZE, 60, 60, 60, 255);
        }

        // Borde de cada caja
        SDL_SetRenderDrawColor(m_Renderer, 200, 200, 200, 200);
        SDL_FRect boxBorder{static_cast<float>(boxX), static_cast<float>(BAR_Y), static_cast<float>(HP_BOX_SIZE), static_cast<float>(HP_BOX_SIZE)};
        SDL_RenderRect(m_Renderer, &boxBorder);
    }

    // Mostrar texto numérico: HP/MaxHP
    const int TEXT_Y = BAR_Y + HP_BOX_SIZE + 8;
}

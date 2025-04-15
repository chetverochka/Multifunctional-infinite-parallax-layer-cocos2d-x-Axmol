#include "InfinityParallax.h"

SpriteFrame* ParallaxSpriteStyles::m_sDefaultSpriteFrame = nullptr;

void ParallaxLayer::applyStylesToAll(const ParallaxSpriteStyles& styles) {
    m_styles = styles;
    for (Sprite* sprite : m_sprites)    applyStylesToTarget(sprite, styles);
}

void ParallaxLayer::applyStylesToTarget(Sprite* target, const ParallaxSpriteStyles& styles) {
    target->setSpriteFrame(styles.m_spriteFrame);
    if (!styles.m_useSpriteFrameRect)
        target->setTextureRect(styles.m_textureRect);
    if (!styles.m_useSpriteFrameSize)
        target->setContentSize(styles.m_contentSize);
    target->setOpacity(styles.m_opacity);
    target->setColor(styles.m_color);
    target->setAnchorPoint(styles.m_anchorPoint);
}

void ParallaxLayer::updateSpritePosition(Sprite* target, Vec2 spriteCell) {
    Vec2 targetPos;
    targetPos.x = (int)spriteCell.x * m_gridStep.width + m_styles.m_spriteOffSet.x + m_gridOffSet.x;
    targetPos.y = (int)spriteCell.y * m_gridStep.height + m_styles.m_spriteOffSet.y + m_gridOffSet.y;

    target->setPosition(targetPos);
}



Sprite* ParallaxLayer::createSprite(Vec2 cell) {
    Sprite* sprite = Sprite::createWithSpriteFrame(m_styles.m_spriteFrame);
    applyStylesToTarget(sprite, m_styles);
    m_sprites.pushBack(sprite);
    m_attachedToCellSpriteMap[cell] = sprite;
    m_spriteCellsMap[sprite] = cell;
    return sprite;
}

void ParallaxLayer::eraseSprite(Sprite* target) {
    bool isCellAttached = m_spriteCellsMap.contains(target);

    if (isCellAttached)
    {
        Vec2 attachedCell = m_spriteCellsMap[target];
        if (m_spriteCellsMap.contains(target))
            m_spriteCellsMap.erase(target);

        if (m_attachedToCellSpriteMap.contains(attachedCell)) {
            m_attachedToCellSpriteMap.erase(attachedCell);
        }
    }

    if (m_sprites.contains(target))
        m_sprites.eraseObject(target);

}

bool InfinityParallax::init() {
    LayerColor::initWithColor(Color4B(0.f,0.f,0.f,0.f));
    m_parallaxPosition = Vec2::ZERO;
    m_viewport = Rect(Vec2::ZERO, Director::getInstance()->getVisibleSize());
    m_velocity = Vec2::ZERO;
    scheduleUpdate();
    return true;
}

void InfinityParallax::addLayer(ParallaxLayer layer) {
    m_layers.push_back(layer);
    updateLayerGrid(m_layers.back(), m_viewport);
}

void InfinityParallax::setParallaxPosition(Vec2 position)
{
    m_parallaxPosition = position;
    for (int i = 0; i < m_layers.size(); i++) {
        ParallaxLayer* layer = &m_layers[i];
        layer->setGridOffSet(position * layer->getSpeedModifier());
        layer->updateAllSpritePositions();
        updateLayerGrid(m_layers[i], m_viewport);
    }
}

void InfinityParallax::update(float dt) {
    moveParallax(Vec2(m_velocity.x * dt, m_velocity.y * dt));
}


void InfinityParallax::updateLayerGrid(ParallaxLayer& layer, Rect visibleArea) {
    Size gridStep = layer.getGridStep();
    Vec2 gridOffset = layer.getGridOffSet();

    int startCellX, startCellY, endCellX, endCellY;

    // left bound
    startCellX = floor((visibleArea.getMinX() - gridOffset.x) / gridStep.width);
    // right bound
    endCellX = ceil((visibleArea.getMaxX() - gridOffset.x) / gridStep.width);

    // bottom bound
    startCellY = floor((visibleArea.origin.y - gridOffset.y) / gridStep.height);
    // top bound
    endCellY = ceil((visibleArea.getMaxY() - gridOffset.y) / gridStep.height);

    cocos2d::Vector<Sprite*>& sprites = layer.getSpritesCCVecRef();
    std::map<Vec2, Sprite*>& attachedToCellSprite = layer.getAttachedToCellSprite();
    std::map<Sprite*, Vec2>& attachedToSpriteCell = layer.getAttachedToSpriteCell();

    // nonvisible sprites cleanup!
    for (int i = 0; i < sprites.size(); i++) {
        Sprite* target = sprites.at(i);
        if (!attachedToSpriteCell.contains(target))  continue;

        Vec2 targetCell = attachedToSpriteCell[target];
        if
           (targetCell.x < startCellX || targetCell.x > endCellX ||
            targetCell.y < startCellY || targetCell.y > endCellY)
        {
            layer.eraseSprite(target);
            target->removeFromParentAndCleanup(true);
        }
    }
    for (int i = startCellY; i < endCellY; i++) {
        for (int j = startCellX; j < endCellX; j++) {
            int targetCellX = j, targetCellY = i;
            Vec2 targetCell(targetCellX, targetCellY);

            bool hasSprite = attachedToCellSprite.contains(targetCell) > 0;
            if (!hasSprite) {
                Sprite* sprite = layer.createSprite(targetCell);
                addChild(sprite, layer.getZIndex());
            }
        }
    }
    layer.updateAllSpritePositions();
}

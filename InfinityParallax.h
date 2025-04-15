#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
using namespace cocos2d;

class InfinityParallax;

struct ParallaxSpriteStyles {
    int m_opacity;
    Color3B m_color;
    Vec2 m_anchorPoint;
    Size m_contentSize;
    Vec2 m_spriteOffSet;
    SpriteFrame* m_spriteFrame;
    Rect m_textureRect;
    bool m_useSpriteFrameRect;
    bool m_useSpriteFrameSize;

    static SpriteFrame* m_sDefaultSpriteFrame;

    ParallaxSpriteStyles() :
        m_opacity(255),
        m_color(Color3B::WHITE),
        m_anchorPoint(Vec2::ZERO),
        m_contentSize(Size::ZERO),
        m_spriteOffSet(Vec2::ZERO),
        m_textureRect(Rect::ZERO),
        m_useSpriteFrameRect(true),
        m_useSpriteFrameSize(true)
    {
        if (!m_sDefaultSpriteFrame)
        {
            Texture2D* frameTexture = Director::getInstance()->getTextureCache()->addImage("HelloWorld.png");
            Rect frameRect;
            frameRect.size = frameTexture->getContentSizeInPixels();
            frameRect.origin = Vec2::ZERO;
            m_sDefaultSpriteFrame = SpriteFrame::createWithTexture(frameTexture, frameRect);
        }
        m_spriteFrame = m_sDefaultSpriteFrame;
    }

    ParallaxSpriteStyles(SpriteFrame* frame) : ParallaxSpriteStyles() {
        m_spriteFrame = frame;
    }
    typedef ui::Widget::TextureResType ResType;
    ParallaxSpriteStyles(std::string path, ResType type) : ParallaxSpriteStyles() {
        setResource(path, type);
    }

    void setResource(std::string path, ResType type) {
        if (type == ResType::LOCAL) {
            Texture2D* frameTexture = Director::getInstance()->getTextureCache()->addImage(path);
            Rect frameRect;
            frameRect.size = frameTexture->getContentSizeInPixels();
            frameRect.origin = Vec2::ZERO;
            m_spriteFrame = SpriteFrame::createWithTexture(frameTexture, frameRect);
        }
        else {      // PLIST
            m_spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(path);
        }
    }
};

class ParallaxLayer {
    CC_SYNTHESIZE(SpriteFrame*, m_spriteFrame, SpriteFrame);
    CC_SYNTHESIZE(Size, m_gridStep, GridStep);
    CC_SYNTHESIZE(Vec2, m_gridOffSet, GridOffSet);
    CC_SYNTHESIZE(Vec2, m_speedModifier, SpeedModifier);
    CC_SYNTHESIZE(int, m_zIndex, ZIndex);
protected:
    Vector<Sprite*> m_sprites;
    std::map<Sprite*, Vec2> m_spriteCellsMap;
    std::map<Vec2, Sprite*> m_attachedToCellSpriteMap;
    ParallaxSpriteStyles m_styles;

    friend class InfinityParallax;
public:
    // sprite getters
    Vector<Sprite*>& getSpritesCCVecRef() { return m_sprites; }
    std::vector<Sprite*> getSprites() { return std::vector<Sprite*>(m_sprites.begin(), m_sprites.end()); }
    std::map<Sprite*, Vec2>& getAttachedToSpriteCell() { return m_spriteCellsMap; }
    std::map<Vec2, Sprite*>& getAttachedToCellSprite() { return m_attachedToCellSpriteMap; }

    // styles
    ParallaxSpriteStyles& getStyles() { return m_styles; }
    void applyStylesToAll(const ParallaxSpriteStyles& styles);
    void applyStylesToTarget(Sprite* target, const ParallaxSpriteStyles& styles);

    // tools
    void updateSpritePosition(Sprite* target, Vec2 spriteCell);
    void updateAllSpritePositions() {
        for (int i = 0; i < m_sprites.size(); i++) {
            updateSpritePosition(m_sprites[i], m_spriteCellsMap[m_sprites[i]]);
        }
    }
    Sprite* createSprite(Vec2 cell);
    void eraseSprite(Sprite* target);
};

class InfinityParallax : public LayerColor {
protected:
    bool init() override;
    void updateLayerGrid(ParallaxLayer& layer, Rect visibleArea);

    // members
    std::vector<ParallaxLayer> m_layers;
    Vec2 m_parallaxPosition;
    Rect m_viewport;
    Vec2 m_velocity;
public:
    CREATE_FUNC(InfinityParallax);
    void update(float dt) override;
    void addLayer(ParallaxLayer layer);
    void setParallaxPosition(Vec2 position);
    Vec2 getParallaxPosition() { return m_parallaxPosition; }
    void moveParallax(Vec2 direction) { setParallaxPosition(getParallaxPosition() + direction); }
    void setVelocity(Vec2 velocity) { m_velocity = velocity; }
    Vec2 getVelocity() { return m_velocity; }
};

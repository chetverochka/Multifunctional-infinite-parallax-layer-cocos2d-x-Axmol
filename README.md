# Multifunctional-infinite-parallax-layer-cocos2d-x-Axmol
A full screen node that allows you to add layers with custom parallax. Compatible with cocos2d-x v4.x / Axmol 2.x

# Usage:
(sorry for my english)

## creating a layer

// create a node:
InfinityParallax* parallax = InfinityParallax::create();

// set automatic scrolling velocity
parallax->setVelocity(Vec2(xx, yy));

// or you can use methods below to move parallax manually (for example relative to player)
parallax->setParallaxPosition(Vec2(xx, yy));
parallax->moveParallax(Vec2(xx, yy)); // same thing as parallax->setParallaxPosition( parallax->getParallaxPosition() + Vec2(moveDir) );

node->addChild(parallax);

## Adding a parallax layer

The layer in this parallax system is an infinite grid of sprites. By moving the parallax you shift the center of the grid. The grid has a cell size, a multiplier, etc.

ParallaxLayer parallaxLayer;
parallaxLayer.setGridStep(parallaxSize); // setup cell size

// The speed multiplier is a multiplier that is multiplied by the target parallax position. 
// This is needed to slow down the movement of layers to create the effect of depth. You can disable the layer's movement along the X/Y axis by setting the multiplier to 0.f.
parallaxLayer.setSpeedModifier(Vec2(0.3f,0.3f));
parallaxLayer.setZIndex(1);

// Styles are applied to layers to change the properties of the sprite. For example, color, offset, size. 
// In styles, you will definitely need to specify the sprite frame for your layer's sprite
ParallaxSpriteStyles parallaxLayerStyles = ParallaxSpriteStyles("HelloWorld.png", ui::Widget::TextureResType::LOCAL); // ResType: PLIST / LOCAL
parallaxLayerStyles.m_useSpriteFrameSize = false;
parallaxLayerStyles.m_contentSize = parallaxSize;

parallaxLayer.applyStylesToAll(parallaxLayerStyles);
parallax->addLayer(parallaxLayer);




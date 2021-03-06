#include "Sprite3DEffect.h"
#include "3d/CCMesh.h"

static int tuple_sort(const std::tuple<ssize_t,Effect3D*,CustomCommand> &tuple1,const std::tuple<ssize_t,Effect3D*,CustomCommand> &tuple2)
{
	return std::get<0>(tuple1) < std::get<0>(tuple2);
}

Effect3DOutline* Effect3DOutline::create()
{
	Effect3DOutline* effect = new (std::nothrow) Effect3DOutline();
	if (effect && effect->init())
	{
		effect->autorelease();
		return effect;
	}
	CC_SAFE_DELETE(effect);
	return nullptr;
}

void Effect3DOutline::setOutLineColor(const Vec3& color)
{
	if (_outlineColor != color)
	{
		_outlineColor = color;
		_glProgramState->setUniformVec3("OutLineColor", _outlineColor);
	}
}

void Effect3DOutline::setOutLineWidth(float width)
{
	if (_outlineWidth != width)
	{
		_outlineWidth = width;
		_glProgramState->setUniformFloat("OutlineWidth", _outlineWidth);
	}
}

void Effect3DOutline::drawWithSprite(EffectSprite3D* sprite, const Mat4& transform)
{
	auto mesh = sprite->getMesh();
	int offset = 0;
	for (auto i = 0; i < mesh->getMeshVertexAttribCount(); i++)
	{
		auto meshvertexattrib = mesh->getMeshVertexAttribute(i);

		_glProgramState->setVertexAttribPointer(s_attributeNames[meshvertexattrib.vertexAttrib], meshvertexattrib.size, meshvertexattrib.type, GL_FALSE, mesh->getVertexSizeInBytes(), (void*)offset);
		offset += meshvertexattrib.attribSizeBytes;
	}
	//draw
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		glEnable(GL_DEPTH_TEST);
		GL::blendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		Color4F color(sprite->getDisplayedColor());
		color.a = sprite->getDisplayedOpacity() / 255.0f;

		_glProgramState->setUniformVec4("u_color", Vec4(color.r, color.g, color.b, color.a));

		auto mesh = sprite->getMesh();
		glBindBuffer(GL_ARRAY_BUFFER, mesh->getVertexBuffer());
		_glProgramState->apply(transform);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->getIndexBuffer());
		glDrawElements((GLenum)mesh->getPrimitiveType(), mesh->getIndexCount(), (GLenum)mesh->getIndexFormat(), (GLvoid*)0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		glCullFace(GL_BACK);
		glDisable(GL_CULL_FACE);
	}
}

Effect3DOutline::Effect3DOutline()
: _outlineColor(1,1,1)
, _outlineWidth(1.0f)
{

}

Effect3DOutline::~Effect3DOutline()
{

}

bool Effect3DOutline::init()
{
	GLProgram* glprogram = Effect3DOutline::getOrCreateProgram();
	if(nullptr == glprogram)
	{
		CC_SAFE_DELETE(glprogram);
		return false;
	}
	_glProgramState = GLProgramState::create(glprogram);
	if(nullptr == _glProgramState)
	{
		return false;
	}
	_glProgramState->retain();
	_glProgramState->setUniformVec3("OutLineColor", _outlineColor);
	_glProgramState->setUniformFloat("OutlineWidth", _outlineWidth);

	return true;
}

const std::string Effect3DOutline::_vertShaderFile = "Shaders3D/OutLine.vert";
const std::string Effect3DOutline::_fragShaderFile = "Shaders3D/OutLine.frag";
const std::string Effect3DOutline::_keyInGLProgramCache = "Effect3DLibrary_Outline";
GLProgram* Effect3DOutline::getOrCreateProgram()
{
	auto program = GLProgramCache::getInstance()->getGLProgram(_keyInGLProgramCache);
	if(program == nullptr)
	{
		program = GLProgram::createWithFilenames(_vertShaderFile, _fragShaderFile);
		GLProgramCache::getInstance()->addGLProgram(program, _keyInGLProgramCache);
	}
	return program;
}

EffectSprite3D* EffectSprite3D::createFromObjFileAndTexture(const std::string& objFilePath, const std::string& textureFilePath)
{
	auto sprite = new (std::nothrow) EffectSprite3D();
	if (sprite && sprite->initWithFile(objFilePath))
	{
		sprite->autorelease();
		if (textureFilePath.size() > 0)
		{
			sprite->setTexture(textureFilePath);
		}
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

EffectSprite3D* EffectSprite3D::create(const std::string &modelPath)
{
	if (modelPath.length() < 4)
	{
		CCASSERT(false, "improper name specified when creating Sprite3D");
	}
	auto sprite = new (std::nothrow) EffectSprite3D();
	if (sprite && sprite->initWithFile(modelPath))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

void EffectSprite3D::setEffect3D(Effect3D* effect)
{
	if(_defaultEffect == effect) return;
	CC_SAFE_RETAIN(effect);
	CC_SAFE_RELEASE(_defaultEffect);
	_defaultEffect = effect;
}

void EffectSprite3D::addEffect(Effect3D* effect, ssize_t order)
{
	if(nullptr == effect) return;
	effect->retain();

	_effects.push_back(std::make_tuple(order,effect,CustomCommand()));

	std::sort(std::begin(_effects), std::end(_effects), tuple_sort);
}

void EffectSprite3D::eraseEffect(Effect3D* effect)
{
	auto iter = _effects.begin();
	for (; iter != _effects.end(); ++iter)
	{
		if(std::get<1>(*iter) != effect) continue;
		else
		{
			CC_SAFE_RELEASE_NULL(std::get<1>(*iter));
			_effects.erase(iter);
			break;
		}

	}
	if(iter == _effects.end())
		CCLOGWARN("Cannot find the effect in EffectSprite3D");
}

ssize_t EffectSprite3D::getEffectCount() const
{
	return _effects.size();
}

Effect3D* EffectSprite3D::getEffect(ssize_t index) const
{
	if (index >= getEffectCount())
	{
		return nullptr;
	}
	return std::get<1>(*(std::begin(_effects) + index));
}

void EffectSprite3D::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	for(auto &effect : _effects)
	{
		if(std::get<0>(effect) >=0)
			break;
		CustomCommand &cc = std::get<2>(effect);
		cc.func = CC_CALLBACK_0(Effect3D::drawWithSprite,std::get<1>(effect),this,transform);
		renderer->addCommand(&cc);

	}

	if(!_defaultEffect)
	{
		Sprite3D::draw(renderer, transform, flags);
	}
	else
	{
		_command.init(_globalZOrder);
		_command.func = CC_CALLBACK_0(Effect3D::drawWithSprite, _defaultEffect, this, transform);
		renderer->addCommand(&_command);
	}

	for(auto &effect : _effects)
	{
		if(std::get<0>(effect) <=0)
			continue;
		CustomCommand &cc = std::get<2>(effect);
		cc.func = CC_CALLBACK_0(Effect3D::drawWithSprite,std::get<1>(effect),this,transform);
		renderer->addCommand(&cc);

	}
}

EffectSprite3D::EffectSprite3D()
	: _defaultEffect(nullptr)
{

}

EffectSprite3D::~EffectSprite3D()
{
	for(auto effect : _effects)
	{
		CC_SAFE_RELEASE_NULL(std::get<1>(effect));
	}
	CC_SAFE_RELEASE(_defaultEffect);
}
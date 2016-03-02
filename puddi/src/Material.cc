#include "Material.h"
#include <vector>

namespace puddi
{
	// PUBLIC

	Material Material::Vibrant(const vec4& color)
	{
        Material m;
		m.ambient = vec4(1.0, 1.0, 1.0, 1.0) * color;
		m.diffuse = vec4(0.55, 0.55, 0.55, 1.0) * color;
		m.specular = vec4(0.7, 0.7, 0.7, 1.0) * color;
		m.shininess = .25f*128.0f;
		return m;
	}

	Material Material::Medium(const vec4& color)
	{
        Material m;
		m.ambient = vec4(0.5, 0.5, 0.5, 1.0) * color;
		m.diffuse = vec4(0.55, 0.55, 0.55, 1.0) * color;
		m.specular = vec4(0.7, 0.7, 0.7, 1.0) * color;
		m.shininess = .25f*128.0f;
		return m;
	}

	Material Material::Plastic(const vec4& color)
	{
		Material m;
		m.ambient = vec4(0.0, 0.0, 0.0, 1.0) * color;
		m.diffuse = vec4(0.55, 0.55, 0.55, 1.0) * color;
		m.specular = vec4(0.7, 0.7, 0.7, 1.0) * color;
		m.shininess = .25f*128.0f;
		return m;
	}

	Material Material::Rubber(const vec4& color)
	{
		Material m;
		m.ambient = vec4(0.05, 0.05, 0.05, 1.0) * color;
		m.diffuse = vec4(0.5, 0.5, 0.5, 1.0) * color;
		m.specular = vec4(0.7, 0.7, 0.7, 1.0) * color;
		m.shininess = .078125f*128.0f;
		return m;
	}

	Material Material::None()
	{
		Material m;
		m.ambient = vec4(0.0f, 0.0f, 0.0f, 0.0f);
		m.diffuse = vec4(0.0f, 0.0f, 0.0f, 0.0f);
		m.specular = vec4(0.0f, 0.0f, 0.0f, 0.0f);
		m.shininess = 0.0f;
		return m;
	}

	Material Material::Pewter()
	{
		Material m;
		m.ambient = vec4(0.105882, 0.058824, 0.113725, 1.0);
		m.diffuse = vec4(0.427451, 0.470588, 0.541176, 1.0);
		m.specular = vec4(0.333333, 0.333333, 0.521569, 1.0);
		m.shininess = 9.84615f;
		return m;
	}

	Material Material::Chrome()
	{
		Material m;
		m.ambient = vec4(0.25, 0.25, 0.25, 1.0);
		m.diffuse = vec4(0.4, 0.4, 0.4, 1.0);
		m.specular = vec4(0.774597, 0.774597, 0.774597, 1.0);
		m.shininess = 76.8f;
		return m;
	}

	Material Material::Obsidian()
	{
		Material m;
		m.ambient = vec4(0.05375, 0.05, 0.06625, 0.82);
		m.diffuse = vec4(0.18275, 0.17, 0.22525, 0.82);
		m.specular = vec4(0.332741, 0.328634, 0.346435, 0.82);
		m.shininess = 38.4f;
		return m;
	}

	Material Material::Silver()
	{
		Material m;
		m.ambient = vec4(0.19225, 0.19225, 0.19225, 1);
		m.diffuse = vec4(0.50754, 0.50754, 0.50754, 1);
		m.specular = vec4(0.508273, 0.508273, 0.508273, 1);
		m.shininess = 51.2f;
		return m;
	}

	Material Material::Bronze()
	{
		Material m;
		m.ambient = vec4(0.2125, 0.1275, 0.054, 1);
		m.diffuse = vec4(0.714, 0.4284, 0.18144, 1);
		m.specular = vec4(0.393548, 0.271906, 0.166721, 1);
		m.shininess = 25.6f;
		return m;
	}

	Material Material::Brass()
	{
		Material m;
		m.ambient = vec4(0.329412, 0.223529, 0.027451, 1);
		m.diffuse = vec4(0.780392, 0.568627, 0.113725, 1);
		m.specular = vec4(0.992157, 0.941176, 0.807843, 1);
		m.shininess = 0.21794872f*128.0f;
		return m;
	}

	Material Material::Pearl()
	{
		Material m;
		m.ambient = vec4(0.25, 0.20725, 0.20725, 1);
		m.diffuse = vec4(1, 0.829, 0.829, 1);
		m.specular = vec4(0.296648, 0.296648, 0.296648, 1);
		m.shininess = 0.088f*128.0f;
		return m;
	}

	Material Material::WhitePlastic()
	{
		Material m;
		m.ambient = vec4(0.0, 0.0, 0.0, 1.0);
		m.diffuse = vec4(0.55, 0.55, 0.55, 1.0);
		m.specular = vec4(0.7, 0.7, 0.7, 1.0);
		m.shininess = 32.0f;
		return m;
	}

	Material Material::BlackPlastic()
	{
		Material m;
		m.ambient = vec4(0.0, 0.0, 0.0, 1.0);
		m.diffuse = vec4(0.01, 0.01, 0.01, 1.0);
		m.specular = vec4(0.5, 0.5, 0.5, 1.0);
		m.shininess = 32.0f;
		return m;
	}

	Material Material::CyanPlastic()
	{
		Material m;
		m.ambient = vec4(0.0, 0.1, 0.06, 1.0);
		m.diffuse = vec4(0.0, 0.50980392, 0.50980392, 1.0);
		m.specular = vec4(0.50196078, 0.50196078, 0.50196078, 1.0);
		m.shininess = 32.0f;
		return m;
	}

	Material Material::GreenPlastic()
	{
		Material m;
		m.ambient = vec4(0.0, 0.0, 0.0, 1.0);
		m.diffuse = vec4(0.1, 0.35, 0.1, 1.0);
		m.specular = vec4(0.45, 0.55, 0.45, 1.0);
		m.shininess = 32.0f;
		return m;
	}

	Material Material::RedPlastic()
	{
		Material m;
		m.ambient = vec4(0.0, 0.0, 0.0, 1.0);
		m.diffuse = vec4(0.5, 0.0, 0.0, 1.0);
		m.specular = vec4(0.7, 0.6, 0.6, 1.0);
		m.shininess = 32.0f;
		return m;
	}

	Material Material::YellowPlastic()
	{
		Material m;
		m.ambient = vec4(0.0, 0.0, 0.0, 1.0);
		m.diffuse = vec4(0.5, 0.5, 0.0, 1.0);
		m.specular = vec4(0.6, 0.6, 0.5, 1.0);
		m.shininess = 32.0f;
		return m;
	}

	Material Material::BlackRubber()
	{
		Material m;
		m.ambient = vec4(0.02, 0.02, 0.02, 1.0);
		m.diffuse = vec4(0.01, 0.01, 0.01, 1.0);
		m.specular = vec4(0.4, 0.4, 0.4, 1.0);
		m.shininess = 10.0f;
		return m;
	}

	Material Material::WhiteRubber()
	{
		Material m;
		m.ambient = vec4(0.05, 0.05, 0.05, 1.0);
		m.diffuse = vec4(0.5, 0.5, 0.5, 1.0);
		m.specular = vec4(0.7, 0.7, 0.7, 1.0);
		m.shininess = 10.0f;
		return m;
	}

	Material Material::CyanRubber()
	{
		Material m;
		m.ambient = vec4(0.0, 0.05, 0.05, 1.0);
		m.diffuse = vec4(0.4, 0.5, 0.5, 1.0);
		m.specular = vec4(0.04, 0.7, 0.7, 1.0);
		m.shininess = 10.0f;
		return m;
	}

	Material Material::GreenRubber()
	{
		Material m;
		m.ambient = vec4(0.0, 0.05, 0.0, 1.0);
		m.diffuse = vec4(0.4, 0.5, 0.4, 1.0);
		m.specular = vec4(0.04, 0.7, 0.04, 1.0);
		m.shininess = 10.0f;
		return m;
	}

	Material Material::RedRubber()
	{
		Material m;
		m.ambient = vec4(0.05, 0.0, 0.0, 1.0);
		m.diffuse = vec4(0.5, 0.4, 0.4, 1.0);
		m.specular = vec4(0.7, 0.04, 0.04, 1.0);
		m.shininess = 10.0f;
		return m;
	}

	Material Material::YellowRubber()
	{
		Material m;
		m.ambient = vec4(0.05, 0.05, 0.0, 1.0);
		m.diffuse = vec4(0.5, 0.5, 0.4, 1.0);
		m.specular = vec4(0.7, 0.7, 0.04, 1.0);
		m.shininess = 10.0f;
		return m;
	}

	bool Material::operator==(const Material& other) const
	{
		return (ambient == other.ambient
			&& diffuse == other.diffuse
			&& specular == other.specular
			&& shininess == other.shininess);
	}
}

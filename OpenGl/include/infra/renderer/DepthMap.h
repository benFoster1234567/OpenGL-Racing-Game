#include <GL/glew.h>


namespace Engine::Infra
{
	class DepthMap
	{
	private:
		GLuint m_depthMap{ 0 };
		size_t m_widthPx{ 0 };
		size_t m_heightPx{ 0 };

		void genTexture()
		{
			if (m_depthMap != 0) return;

			glGenTextures(1, &m_depthMap);
			glBindTexture(GL_TEXTURE_2D, m_depthMap);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
				static_cast<GLsizei>(m_widthPx), static_cast<GLsizei>(m_heightPx),
				0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		}

	public:
		DepthMap(size_t width, size_t height) : m_widthPx{ width }, m_heightPx{ height } {}
		~DepthMap() { destroy(); }

		DepthMap(const DepthMap&) = delete;
		DepthMap& operator=(const DepthMap&) = delete;

		DepthMap(DepthMap&& other) noexcept
			: m_depthMap(other.m_depthMap), m_widthPx(other.m_widthPx), m_heightPx(other.m_heightPx)
		{
			other.m_depthMap = 0;
		}

		DepthMap& operator=(DepthMap&& other) noexcept
		{
			if (this != &other) {
				destroy();
				m_depthMap = other.m_depthMap;
				m_widthPx = other.m_widthPx;
				m_heightPx = other.m_heightPx;
				other.m_depthMap = 0;
			}
			return *this;
		}

		void create() { genTexture(); }
		void destroy()
		{
			if (m_depthMap != 0) {
				glDeleteTextures(1, &m_depthMap);
				m_depthMap = 0;
			}
		}

		void bind(GLuint unit = 0) const
		{
			glActiveTexture(GL_TEXTURE0 + unit);
			glBindTexture(GL_TEXTURE_2D, m_depthMap);
		}

		void attach() const {
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthMap, 0);
		}
	};
}
namespace Engine::Infra
{
    class DepthMap
    {
    private:
        GLuint depthMap{ 0 };
        size_t widthPx{ 0 };
        size_t heightPx{ 0 };

        void genTexture()
        {
            if (depthMap != 0) return;

            glGenTextures(1, &depthMap);
            glBindTexture(GL_TEXTURE_2D, depthMap);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                static_cast<GLsizei>(widthPx), static_cast<GLsizei>(heightPx),
                0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
            float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        }

    public:
        DepthMap(size_t width, size_t height) : widthPx{ width }, heightPx{ height } {}
        ~DepthMap() { destroy(); }

        DepthMap(const DepthMap&) = delete;
        DepthMap& operator=(const DepthMap&) = delete;

        DepthMap(DepthMap&& other) noexcept
            : depthMap(other.depthMap), widthPx(other.widthPx), heightPx(other.heightPx)
        {
            other.depthMap = 0;
        }

        DepthMap& operator=(DepthMap&& other) noexcept
        {
            if (this != &other) {
                destroy();
                depthMap = other.depthMap;
                widthPx = other.widthPx;
                heightPx = other.heightPx;
                other.depthMap = 0;
            }
            return *this;
        }

        void create() { genTexture(); }
        void destroy()
        {
            if (depthMap != 0) {
                glDeleteTextures(1, &depthMap);
                depthMap = 0;
            }
        }

        void bind(GLuint unit = 0) const
        {
            glActiveTexture(GL_TEXTURE0 + unit);
            glBindTexture(GL_TEXTURE_2D, depthMap);
        }

        void attach() const {
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
        }
    };
}
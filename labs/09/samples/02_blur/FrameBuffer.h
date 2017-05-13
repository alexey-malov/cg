#pragma once

// ������� ����� "����� �����"
class CFrameBufferBase
{
public:
	// �������� ������ ������ �����
	void Create()
	{
		assert(!m_frameBuffer);
		glGenRenderbuffersEXT(1, &m_frameBuffer);
	}

	// �������� ������ ����� � ��������� ����
	static void Bind(GLuint target, GLuint framebuffer)
	{
		glBindFramebufferEXT(target, framebuffer);
	}

	// �������� �������� ������ �����
	void Bind(GLenum target = GL_FRAMEBUFFER_EXT)const
	{
		assert(m_frameBuffer != 0);
		glBindFramebufferEXT(target, m_frameBuffer);
	}

	// ������������� ������ ������������ � ������ �����
	static void SetRenderBuffer(
		GLenum target, GLenum attachment, 
		GLenum renderbuffertarget, GLuint renderbuffer)
	{
		glFramebufferRenderbufferEXT(
			target, attachment, renderbuffertarget, renderbuffer);
	}

	// ������������� ���������� �������� � ������ �����
	static void SetTexture2D(
		GLenum target, GLenum attachment, 
		GLenum textarget, GLuint texture, GLint level)
	{
		glFramebufferTexture2DEXT(
			target, attachment, textarget, texture, level);
	}

	// �������� ������� ������ �����
	static GLenum CheckStatus(GLenum target)
	{
		return glCheckFramebufferStatusEXT(target);
	}

	// �������� ������ �����
	void Delete()
	{
		assert(m_frameBuffer != 0);
		glDeleteFramebuffersEXT(1, &m_frameBuffer);
		m_frameBuffer = 0;
	}

	// ��������� �������������� ������ �����
	GLuint Get()const
	{
		return m_frameBuffer;
	}

	// ��������� �������������� ������ �����
	operator GLuint()const
	{
		return m_frameBuffer;
	}
protected:
	CFrameBufferBase(GLuint frameBuffer = 0)
		:m_frameBuffer(0)
	{
	}

	~CFrameBufferBase()
	{
	}

	// ������� �������������� ������ �����
	void SetFrameBuffer(GLuint frameBuffer)
	{
		m_frameBuffer = frameBuffer;
	}
private:
	// ��������� ����������� � ������������
	// ����������� ������ CFrameBufferBase
	CFrameBufferBase(CFrameBufferBase const&);
	CFrameBufferBase& operator=(CFrameBufferBase const&);

	GLuint m_frameBuffer;
};

// ����������� ��� ������������� ���������� ������ �����
template <bool t_managed>
class CFrameBufferImpl : public CFrameBufferBase
{
public:
	CFrameBufferImpl(GLuint frameBuffer = 0)
		:CFrameBufferBase(frameBuffer)
	{
	}

	~CFrameBufferImpl()
	{
		// ������� ����� �����, ���� ����� ��������� ��������
		// ��� �����
		if (t_managed && (Get() != 0))
		{
			Delete();
		}
	}
private:
};

// ����� ����� (����� ��������� �������� ����� ������� OpenGL)
typedef CFrameBufferImpl<true> CFrameBuffer;

// ���������� ������ ����� (����� �� ��������� �������� 
// ����� ������� OpenGL)
typedef CFrameBufferImpl<false> CFrameBufferHandle;

// ������� ����� ������������
class CRenderBufferBase
{
public:
	// �������� ������ ������������
	void Create()
	{
		assert(!m_renderBuffer);
		glGenRenderbuffersEXT(1, &m_renderBuffer);
	}

	// �������� � �������� ������ ������������
	void Bind(GLenum target = GL_RENDERBUFFER_EXT)const
	{
		assert(m_renderBuffer != 0);
		glBindRenderbufferEXT(target, m_renderBuffer);
	}

	// �������� � ��������� ������ ������������
	static void Bind(GLenum target, GLenum renderBuffer)
	{
		glBindRenderbufferEXT(target, renderBuffer);
	}

	// �������� ������ ������������
	void Delete()
	{
		assert(m_renderBuffer != 0);
		glDeleteRenderbuffersEXT(1, &m_renderBuffer);
		m_renderBuffer = 0;
	}

	// ��������� �������������� ������ ������������
	GLuint Get()const
	{
		return m_renderBuffer;
	}

	// ��������� �������������� ������ ������������
	operator GLuint()const
	{
		return m_renderBuffer;
	}

	// ��������� ������ ��� �������� ������ ����� ��������� �������
	static void SetStorage(
		GLenum target, GLenum internalformat, 
		GLsizei width, GLsizei height)
	{
		glRenderbufferStorageEXT(target, internalformat, width, height);
	}

protected:
	CRenderBufferBase(GLuint renderBuffer = 0)
		:m_renderBuffer(renderBuffer)
	{
	}
	void SetRenderBuffer(GLuint renderBuffer)
	{
		m_renderBuffer = renderBuffer;
	}
	~CRenderBufferBase()
	{
	}
private:
	// ��������� ����������� ����������� � �������� ������������
	CRenderBufferBase(CRenderBufferBase const&);
	CRenderBufferBase& operator=(CRenderBufferBase const&);

	GLuint m_renderBuffer;
};

// �����������, ���� ������������� ���������� 
// ������ "����� ������������"
template <bool t_managed>
class CRenderBufferImpl : public CRenderBufferBase
{
public:
	CRenderBufferImpl(GLuint renderBuffer = 0)
		:CRenderBufferBase(renderBuffer)
	{
	}

	~CRenderBufferImpl()
	{
		// ������� ����� ������������
		if (t_managed && (Get() != 0))
		{
			Delete();
		}
	}
};

// ����� ������������ (����� ��������� �������� ����� ������� OpenGL)
typedef CRenderBufferImpl<true> CRenderBuffer;

// ���������� ������ ������������
// (����� �� ��������� �������� ����� ������� OpenGL)
typedef CRenderBufferImpl<false> CRenderBufferHandle;

#pragma once
#include <Windows.h>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>

namespace FL {
	template<class T>
	class Singleton {
	protected:
		__forceinline Singleton()noexcept = default;
		__forceinline virtual ~Singleton()noexcept = default;
	public:
		static T* GetInstance() {
			static T inst;
			return &inst;
		}
	};
	template<class T>
	__forceinline void SafeDelete(T* p) {
		if (p) {
			delete p;
			p = nullptr;
		}
	}
#ifdef _DEBUG
	//これは例外発生時に指定された方式で情報を表示する
#define __EXCEPTION_ON__
#else
#define __EXCEPTION_OFF__
#endif
	/**
	*@brief 例外クラス
	*@brief 一応地震で実体を作って使用することも可能
	*@attention このクラスは自分で実体を生成するのではなく、<br>
	*下で定義されているマクロによって生成させる。<br>
	*メッセージを表示するには、CATCH文でこのクラスを捕まえてメンバ関数によって表示
	*@attention ログ出力等の制御は上にあるコメント参照
	*/
	class Exception {
	private:
		/**@brief ソースファイル名*/
		std::string _filename;
		/**@brief 実行された関数名*/
		std::string _functionName;
		/**@brief 例外が出た行*/
		int _executeLine;
		/**@brief 任意のメッセージ*/
		std::string _msg;
	private:
		/**
		*@brief エラーメッセージ構築
		*@return エラー文章
		*/
		__forceinline std::string CreateMessage()const noexcept {
			std::stringstream error;
			error << "\n======! begin exception message !======\n\n" << "\n[source : " << _filename << "]\n\n[function : " << _functionName << "]\n\n[line : " << _executeLine << "]\n\n";
			if (!_msg.empty())error << "[cause : " << _msg << "]\n\n";
			error << "\n======! end exception message !======\n\n";
			return error.str();
		}
	public:
		/**
		*@brief コンストラクタ
		*@param[in] ソースファイル名
		*@param[in] 関数名
		*@param[in] 実行行番号
		*/
		__forceinline  Exception(std::string filename, std::string func_name, int exe_line) noexcept :Exception(filename, func_name, exe_line, "") {}
		/**
		*@brief コンストラクタ
		*@param[in] ソースファイル名
		*@param[in] 関数名
		*@param[in] 実行行番号
		*@param[in] 任意のメッセージ
		*/
		__forceinline Exception(std::string filename, std::string func_name, int exe_line, std::string msg) noexcept : _filename(filename), _functionName(func_name), _executeLine(exe_line), _msg(msg) {}
		/**@brief デストラクタ*/
		__forceinline virtual ~Exception() noexcept {}
		/**@brief コンソールに出力*/
		__forceinline void ConsoleMessage()const noexcept {
#ifdef __EXCEPTION_ON__
			std::cout << CreateMessage() << std::endl;
#endif
		}
		/**@brief VisualStudioの出力窓に出力*/
		__forceinline void VsConsoleMessage()const noexcept {
#ifdef __EXCEPTION_ON__
			OutputDebugString(CreateMessage().c_str());
#endif
		}
		/**@brief メッセージボックスにより出力*/
		__forceinline void BoxMessage()const noexcept {
#ifdef __EXCEPTION_ON__
			MessageBox(nullptr, CreateMessage().c_str(), "exception!", MB_OK | MB_ICONERROR);
#endif
		}
		/**@brief エラー文章を取得*/
		__forceinline const std::string GetExceptionMessage()const noexcept {
			return CreateMessage();
		}
	};
}
//この先マクロ定義
/**@def ソースファイル名取得*/
#define FILE_NAME __FILE__
/**@def 関数名取得*/
#define FUNCTION_NAME __func__
/**@def 実行している行取得*/
#define EXECUTE_LINE __LINE__
/**@def 変数名取得*/
#define VARIABLE_NAME(variable) # variable
/**@def 例外クラス実体生成*/
#define STRICT_EXCEPTION_FACTORY(msg) FL::Exception(FILE_NAME,FUNCTION_NAME,EXECUTE_LINE,msg)
/**@def 例外クラス実体生成*/
#define EXCEPTION_FACTORY STRICT_EXCEPTION_FACTORY("")
/**@def try*/
#define TRY try
/**@def throw Exceptionをメッセージ付きで投げる*/
#define STRICT_THROW(msg) throw STRICT_EXCEPTION_FACTORY(msg)
/**@def throw Exceptionを投げる*/
#define THROW_E STRICT_THROW("");
/**@def catch*/
#define CATCH catch
/**@def throw*/
#define THROW throw
/**@def アライメント*/
#define ALIGN(n) __declspec(align(n))

/**
*@brief floatの等値判定
*@param[in] 左辺値
*@param[in] 右辺値
*@param[in] 許容値 デフォルトではものすごく小さい値が設定される
*/
__forceinline bool FloatEqual(float x, float y, float threshold = FLT_EPSILON) noexcept { return  (fabsf(x - y) <= threshold); };


class Vector2 {
public:
	union {
		struct { float x, y; };
		float v[2];
	};
	/**
	*@brief 型がintのVector2<br>
	*値を取る為のみ使用なので、メンバ関数は提供しない
	*/
	struct IVector2 {
		union {
			struct { int x, y; };
			int v[2];
		};
		__forceinline IVector2(int x, int y) :x(x), y(y) {}
	};
	__forceinline constexpr Vector2(float x, float y) noexcept : x(x), y(y) {}
	__forceinline Vector2(const IVector2& v) noexcept : Vector2(static_cast<float>(v.x), static_cast<float>(v.y)) {}
	__forceinline constexpr Vector2()noexcept : Vector2(0.0f, 0.0f) {}
	/**
	*@brief 値をint型で取得する関数
	*/
	__forceinline IVector2 Get()const  noexcept { return IVector2(static_cast<int>(x), static_cast<int>(y)); }
	/**
	*@brief int型のIVector2からの直接Vector2に代入
	*/
	__forceinline void Set(const IVector2& temp)noexcept {
		for (int i = 0; i < 2; i++) {
			this->v[i] = static_cast<float>(temp.v[i]);
		}
	}
	/**@brief 距離(平方根あり)*/
	__forceinline float Length() { return sqrtf(x*x + y*y); }
	/**@brief 距離(平方根なし)*/
	__forceinline float LengthSq() { return (x*x + y*y); }
	/**@brief 正規化*/
	__forceinline void Normalize() {
		float l = Length();
		if (l != 0.0f) { x /= l;	y /= l; }
	}
	__forceinline bool IsEqual(const Vector2& v) { return (FloatEqual(x, v.x) && FloatEqual(y, v.y)); }
	/**@brief 内積*/
	__forceinline static float Dot(const Vector2& v1, const Vector2& v2) { return (v1.x*v2.x + v1.y*v2.y); }
	__forceinline Vector2 operator +(const Vector2& v) { return Vector2(x + v.x, y + v.y); }
	__forceinline Vector2 operator -(const Vector2& v) { return Vector2(x - v.x, y - v.y); }
	__forceinline Vector2 operator *(const Vector2& v) { return Vector2(x * v.x, y * v.y); }
	__forceinline Vector2 operator /(const Vector2& v) { return Vector2(x / v.x, y / v.y); }
	__forceinline Vector2 operator +(float temp) { return Vector2(x + temp, y + temp); }
	__forceinline Vector2 operator -(float temp) { return Vector2(x - temp, y - temp); }
	__forceinline Vector2 operator *(float temp) { return Vector2(x * temp, y * temp); }
	__forceinline Vector2 operator /(float temp) { return Vector2(x / temp, y / temp); }
	__forceinline Vector2& operator +=(const Vector2& v) { x += v.x;	y += v.y; return *this; }
	__forceinline Vector2& operator -=(const Vector2& v) { x -= v.x;	y -= v.y; return *this; }
	__forceinline Vector2& operator *=(const Vector2& v) { x *= v.x;	y *= v.y; return *this; }
	__forceinline Vector2& operator /=(const Vector2& v) { x /= v.x;	y /= v.y; return *this; }
	__forceinline Vector2& operator +=(float temp) { x += temp;	y += temp; return *this; }
	__forceinline Vector2& operator -=(float temp) { x -= temp;	y -= temp; return *this; }
	__forceinline Vector2& operator *=(float temp) { x *= temp;	y *= temp; return *this; }
	__forceinline Vector2& operator /=(float temp) { x /= temp;	y /= temp; return *this; }
	__forceinline bool operator ==(const Vector2& v) { return IsEqual(v); }
	__forceinline bool operator !=(const Vector2& v) { return !IsEqual(v); }
};
__forceinline Vector2 operator +(const Vector2& v1, const Vector2& v2) { return Vector2(v1.x + v2.x, v1.y + v2.y); }
__forceinline Vector2 operator -(const Vector2& v1, const Vector2& v2) { return Vector2(v1.x - v2.x, v1.y - v2.y); }
__forceinline Vector2 operator *(const Vector2& v1, const Vector2& v2) { return Vector2(v1.x * v2.x, v1.y * v2.y); }
__forceinline Vector2 operator /(const Vector2& v1, const Vector2& v2) { return Vector2(v1.x / v2.x, v1.y / v2.y); }
__forceinline Vector2 operator +(const Vector2& v1, float temp) { return Vector2(v1.x + temp, v1.y + temp); }
__forceinline Vector2 operator -(const Vector2& v1, float temp) { return Vector2(v1.x - temp, v1.y - temp); }
__forceinline Vector2 operator *(const Vector2& v1, float temp) { return Vector2(v1.x * temp, v1.y * temp); }
__forceinline Vector2 operator /(const Vector2& v1, float temp) { return Vector2(v1.x / temp, v1.y / temp); }

class Vector3 {
public:
	union {
		struct { float x, y, z; };
		float v[3];
	};
	/**
	*@brief 型がintのVector3<br>
	*値を取る為のみ使用なので、メンバ関数は提供しない
	*/
	struct IVector3 {
		union {
			struct { int x, y, z; };
			int v[3];
		};
		__forceinline IVector3(int x, int y, int z) :x(x), y(y), z(z) {}
	};
	__forceinline constexpr Vector3(float x, float y, float z) noexcept : x(x), y(y), z(z) {}
	__forceinline Vector3(const IVector3& v) noexcept : Vector3(static_cast<float>(v.x), static_cast<float>(v.y), static_cast<float>(v.z)) {}
	__forceinline constexpr Vector3()noexcept : Vector3(0.0f, 0.0f, 0.0f) {}
	/**
	*@brief 値をint型で取得する関数
	*/
	__forceinline IVector3 Get()const  noexcept { return IVector3(static_cast<int>(x), static_cast<int>(y), static_cast<int>(z)); }
	/**
	*@brief int型のIVector3からの直接Vector3に代入
	*/
	__forceinline void Set(const IVector3& v)noexcept {
		for (int i = 0; i < 3; i++) {
			this->v[i] = static_cast<float>(v.v[i]);
		}
	}
	/**@brief 距離(平方根あり)*/
	__forceinline float Length() { return sqrtf(x*x + y*y + z*z); }
	/**@brief 距離(平方根なし)*/
	__forceinline float LengthSq() { return (x*x + y*y + z*z); }
	/**@brief 正規化*/
	__forceinline void Normalize() {
		float l = Length();
		if (l != 0.0f) { x /= l;	y /= l; z /= l; }
	}
	__forceinline bool IsEqual(const Vector3& v) { return (FloatEqual(x, v.x) && FloatEqual(y, v.y) && FloatEqual(z, v.z)); }
	/**@brief 内積*/
	__forceinline static float Dot(const Vector3& v1, const Vector3& v2) { return (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z); }
	/**@brief 外積*/
	__forceinline static Vector3 Cross(const Vector3& v1, const Vector3& v2) {
		Vector3 ret = {};
		ret.x = v1.y*v2.z - v1.z*v2.y;
		ret.y = v1.z*v2.x - v1.x*v2.z;
		ret.z = v1.x*v2.y - v1.y*v2.x;
		return ret;
	}
	__forceinline Vector3 operator +(const Vector3& v) { return Vector3(x + v.x, y + v.y, z + v.z); }
	__forceinline Vector3 operator -(const Vector3& v) { return Vector3(x - v.x, y - v.y, z - v.z); }
	__forceinline Vector3 operator *(const Vector3& v) { return Vector3(x * v.x, y * v.y, z * v.z); }
	__forceinline Vector3 operator /(const Vector3& v) { return Vector3(x / v.x, y / v.y, z / v.z); }
	__forceinline Vector3 operator +(float temp) { return Vector3(x + temp, y + temp, z + temp); }
	__forceinline Vector3 operator -(float temp) { return Vector3(x - temp, y - temp, z - temp); }
	__forceinline Vector3 operator *(float temp) { return Vector3(x * temp, y * temp, z * temp); }
	__forceinline Vector3 operator /(float temp) { return Vector3(x / temp, y / temp, z / temp); }
	__forceinline Vector3& operator +=(const Vector3& v) { x += v.x; y += v.y; z += v.z; return *this; }
	__forceinline Vector3& operator -=(const Vector3& v) { x -= v.x;	y -= v.y;	z -= v.z; return *this; }
	__forceinline Vector3& operator *=(const Vector3& v) { x *= v.x;	y *= v.y;	z *= v.z; return *this; }
	__forceinline Vector3& operator /=(const Vector3& v) { x /= v.x;	y /= v.y;	z /= v.z; return *this; }
	__forceinline Vector3& operator +=(float temp) { x += temp;	y += temp;	z += temp; return *this; }
	__forceinline Vector3& operator -=(float temp) { x -= temp;	y -= temp;	z -= temp; return *this; }
	__forceinline Vector3& operator *=(float temp) { x *= temp;	y *= temp;	z *= temp; return *this; }
	__forceinline Vector3& operator /=(float temp) { x /= temp;	y /= temp;	z /= temp; return *this; }
	__forceinline bool operator ==(const Vector3& v) { return IsEqual(v); }
	__forceinline bool operator !=(const Vector3& v) { return !IsEqual(v); }
};
__forceinline Vector3 operator +(const Vector3& v1, const Vector3& v2) { return Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z); }
__forceinline Vector3 operator -(const Vector3& v1, const Vector3& v2) { return Vector3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z); }
__forceinline Vector3 operator *(const Vector3& v1, const Vector3& v2) { return Vector3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z); }
__forceinline Vector3 operator /(const Vector3& v1, const Vector3& v2) { return Vector3(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z); }
__forceinline Vector3 operator +(const Vector3& v1, float temp) { return Vector3(v1.x + temp, v1.y + temp, v1.z + temp); }
__forceinline Vector3 operator -(const Vector3& v1, float temp) { return Vector3(v1.x - temp, v1.y - temp, v1.z - temp); }
__forceinline Vector3 operator *(const Vector3& v1, float temp) { return Vector3(v1.x * temp, v1.y * temp, v1.z * temp); }
__forceinline Vector3 operator /(const Vector3& v1, float temp) { return Vector3(v1.x / temp, v1.y / temp, v1.z / temp); }

class Vector4 {
public:
	union {
		struct { float x, y, z, w; };
		float v[4];
	};
	/**
	*@brief 型がintのVector4<br>
	*値を取る為のみ使用なので、メンバ関数は提供しない
	*/
	struct IVector4 {
		union {
			struct { int x, y, z, w; };
			int v[4];
		};
		__forceinline IVector4(int x, int y, int z, int w) :x(x), y(y), z(z), w(w) {}
	};
	__forceinline constexpr Vector4(float x, float y, float z, float w) noexcept :x(x), y(y), z(z), w(w) {}
	__forceinline Vector4(const IVector4& v) noexcept : Vector4(static_cast<float>(v.x), static_cast<float>(v.y), static_cast<float>(v.z), static_cast<float>(v.w)) {}
	__forceinline constexpr Vector4()noexcept : Vector4(0.0f, 0.0f, 0.0f, 0.0f) {}
	/**
	*@brief 値をint型で取得する関数
	*/
	__forceinline IVector4 Get()const  noexcept { return IVector4(static_cast<int>(x), static_cast<int>(y), static_cast<int>(z), static_cast<int>(w)); }
	/**
	*@brief int型のIVector4からの直接Vector4に代入
	*/
	__forceinline void Set(const IVector4& v)noexcept {
		for (int i = 0; i < 4; i++) {
			this->v[i] = static_cast<float>(v.v[i]);
		}
	}
	/**@brief 距離(平方根あり)*/
	__forceinline float Length() { return sqrtf(x*x + y*y + z*z + w*w); }
	/**@brief 距離(平方根なし)*/
	__forceinline float LengthSq() { return (x*x + y*y + z*z + w*w); }
	/**@brief 正規化*/
	__forceinline void Normalize() {
		float l = Length();
		if (l != 0.0f) { x /= l;	y /= l; z /= l; }
	}
	__forceinline bool IsEqual(const Vector4& v) { return (FloatEqual(x, v.x) && FloatEqual(y, v.y) && FloatEqual(z, v.z) && FloatEqual(w, v.w)); }
	/**@brief 内積*/
	__forceinline static float Dot(const Vector4& v1, const Vector4& v2) { return (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z + v1.w*v2.w); }
	__forceinline Vector4 operator +(const Vector4& v) { return Vector4(x + v.x, y + v.y, z + v.z, w + v.w); }
	__forceinline Vector4 operator -(const Vector4& v) { return Vector4(x - v.x, y - v.y, z - v.z, w - v.w); }
	__forceinline Vector4 operator *(const Vector4& v) { return Vector4(x * v.x, y * v.y, z * v.z, w * v.w); }
	__forceinline Vector4 operator /(const Vector4& v) { return Vector4(x / v.x, y / v.y, z / v.z, w / v.w); }
	__forceinline Vector4 operator +(float temp) { return Vector4(x + temp, y + temp, z + temp, w + temp); }
	__forceinline Vector4 operator -(float temp) { return Vector4(x - temp, y - temp, z - temp, w - temp); }
	__forceinline Vector4 operator *(float temp) { return Vector4(x * temp, y * temp, z * temp, w * temp); }
	__forceinline Vector4 operator /(float temp) { return Vector4(x / temp, y / temp, z / temp, w / temp); }
	__forceinline Vector4& operator +=(const Vector4& v) { x += v.x;	y += v.y;	z += v.z; w += v.w; return *this; }
	__forceinline Vector4& operator -=(const Vector4& v) { x -= v.x;	y -= v.y;	z -= v.z; w -= v.w; return *this; }
	__forceinline Vector4& operator *=(const Vector4& v) { x *= v.x;	y *= v.y;	z *= v.z; w *= v.w; return *this; }
	__forceinline Vector4& operator /=(const Vector4& v) { x /= v.x;	y /= v.y;	z /= v.z; w /= v.w; return *this; }
	__forceinline Vector4& operator +=(float temp) { x += temp;	y += temp;	z += temp; z += temp; return *this; }
	__forceinline Vector4& operator -=(float temp) { x -= temp;	y -= temp;	z -= temp; z -= temp; return *this; }
	__forceinline Vector4& operator *=(float temp) { x *= temp;	y *= temp;	z *= temp; z *= temp; return *this; }
	__forceinline Vector4& operator /=(float temp) { x /= temp;	y /= temp;	z /= temp; z /= temp; return *this; }
	__forceinline bool operator ==(const Vector4& v) { return IsEqual(v); }
	__forceinline bool operator !=(const Vector4& v) { return !IsEqual(v); }
};
__forceinline Vector4 operator +(const Vector4& v1, const Vector4& v2) { return Vector4(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w); }
__forceinline Vector4 operator -(const Vector4& v1, const Vector4& v2) { return Vector4(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w); }
__forceinline Vector4 operator *(const Vector4& v1, const Vector4& v2) { return Vector4(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.w * v2.w); }
__forceinline Vector4 operator /(const Vector4& v1, const Vector4& v2) { return Vector4(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z, v1.w / v2.w); }
__forceinline Vector4 operator +(const Vector4& v1, float temp) { return Vector4(v1.x + temp, v1.y + temp, v1.z + temp, v1.w + temp); }
__forceinline Vector4 operator -(const Vector4& v1, float temp) { return Vector4(v1.x - temp, v1.y - temp, v1.z - temp, v1.w + temp); }
__forceinline Vector4 operator *(const Vector4& v1, float temp) { return Vector4(v1.x * temp, v1.y * temp, v1.z * temp, v1.w + temp); }
__forceinline Vector4 operator /(const Vector4& v1, float temp) { return Vector4(v1.x / temp, v1.y / temp, v1.z / temp, v1.w + temp); }

struct Matrix {
public:
	float mat[4][4] = {};
};


/**
 * \class	OldResourceManager
 * 
 * \ingroup	resources
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 */

#ifndef __OldResourceManager_H__
#define __OldResourceManager_H__

// Includes
#include "Singleton.h"
#include "StringId.h"
#include "HashTable.h"
#include "HashTableKey_Pointer.h"

// Forward declares
class BitmapStrip;
class Font;
class Sound;
class Image;
class Filename;
class Bitmap;
class Asset;
class StaticBuffer;

// OldResourceManager
class OldResourceManager:public Singleton<OldResourceManager>
	{
	public:
		OldResourceManager();
		virtual ~OldResourceManager();

		void LoadBitmapStrip(
			const Filename& filename,
			int celCount = 1
			);

		void UnloadBitmapStrip(
			const Filename& filename
			);
	
		void LoadFont(
			const Filename& filename
			);

		void UnloadFont(
			const Filename& filename
			);

		void LoadSound(
			const Filename& filename
			);

		void UnloadSound(
			const Filename& filename
			);

		void LoadAsset(
			const Filename& filename
			);

		void UnloadAsset(
			const Filename& filename
			);

		const Asset* GetAsset(
			StringId filename
			);

	private:
		friend class Resource_BitmapStrip;
		friend class Resource_Font;
		friend class Resource_Sound;

		const BitmapStrip* GetBitmapStrip(
			StringId filename,
			int celCount = 1
			);

		const BitmapStrip* GetBitmapStrip(
			const Image& image
			);
	
		const BitmapStrip* GetBitmapStrip(
			const Bitmap* bitmap
			);

		const Font* GetFont(
			StringId filename
			);

		const Sound* GetSound(
			StringId filename
			);

		void IncreaseReferenceCount(
			const BitmapStrip* bitmapStrip
			);
		
		void DecreaseReferenceCount(
			const BitmapStrip* bitmapStrip
			);

		void IncreaseReferenceCount(
			const Font* font
			);
		
		void DecreaseReferenceCount(
			const Font* font
			);

		void IncreaseReferenceCount(
			const Sound* sound
			);
		
		void DecreaseReferenceCount(
			const Sound* sound
			);

		void IncreaseReferenceCount(
			const Asset* asset
			);
		
		void DecreaseReferenceCount(
			const Asset* asset
			);

		struct BitmapStripEntry
			{
			StringId filename;
			const BitmapStrip* bitmapStrip;
			int referenceCount;
			};

		BitmapStripEntry* GetBitmapStripEntry(
			const BitmapStrip* bitmapStrip
			);

		struct FontEntry
			{
			StringId filename;
			const Font* font;
			int referenceCount;
			};

		FontEntry* GetFontEntry(
			const Font* font
			);

		struct SoundEntry
			{
			StringId filename;
			const Sound* sound;
			int referenceCount;
			};

		SoundEntry* GetSoundEntry(
			const Sound* sound
			);

		struct AssetEntry
			{
			StringId filename;
			const Asset* asset;
			char* buf;
			StaticBuffer* staticBuffer;
			int referenceCount;
			};

		AssetEntry* GetAssetEntry(
			const Asset* asset
			);

	private:
		HashTable<HashTableKey_Pointer,BitmapStripEntry*> bitmapStrips_;
		HashTable<HashTableKey_Pointer,FontEntry*> fonts_;
		HashTable<HashTableKey_Pointer,SoundEntry*> sounds_;
		HashTable<HashTableKey_Pointer,AssetEntry*> asset_;
	};

#define siOldResourceManager OldResourceManager::GetInstance()

#endif /* __OldResourceManager_H__ */

	
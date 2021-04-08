#pragma once

#include <stdint.h>

namespace Greet
{
  class Random
  {

    private:
      uint32_t index = 0;
      uint32_t seed = 0;

    public:
      Random(uint32_t startIndex, uint32_t seed)
        : index{startIndex}, seed{seed}
      {}
      Random(uint32_t seed = 0)
        : index{0}, seed{seed}
      {}

      inline void Reset(int index=0)
      {
        this->index = index;
      }

      inline void SetSeed(int seed)
      {
        this->seed = seed;
      }

      inline uint32_t GetIndex()
      {
        return index;
      }

      inline uint32_t GetSeed()
      {
        return seed;
      }

      inline bool Int32Between(int32_t min, int32_t max)
      {
        return Int32() % (max - min) + min;
      }

      inline bool Int64Between(int64_t min, int64_t max)
      {
        return Int64() % (max - min) + min;
      }

      inline bool UInt32Between(int32_t min, int32_t max)
      {
        return UInt32() % (max - min) + min;
      }

      inline bool UInt64Between(int64_t min, int64_t max)
      {
        return UInt64() % (max - min) + min;
      }

      // c is a number between 0 - 1
      inline bool Chance(float c)
      {
        float n = Float();
        return n < c;
      }

      inline double FloatBetween(float min, float max)
      {
        return Float() * (max - min) + min;
      }

      inline double DoubleBetween(double min, double max)
      {
        return Double() * (max - min) + min;
      }

      // float between -1 and 1
      inline float FloatN()
      {
        return (Float() - 0.5f) * 2.0f;
      }

      // double between -1 and 1
      inline double DoubleN()
      {
        return (Double() - 0.5) * 2.0;
      }

      // float between 0 and 1
      inline float Float()
      {
        return (float)(UInt32() & 0xffffff) / (float)0x01000000;
      }

      // double between 0 and 1
      inline double Double()
      {
        return (double)(UInt64() & 0xffffffffffff) / (double)0x01000000000000;
      }

      inline int32_t Int8()
      {
        return (int8_t)(UInt32() & 0xff);
      }

      inline int32_t UInt8()
      {
        return (uint8_t)(UInt32() & 0xff);
      }

      inline int32_t Int16()
      {
        return (int16_t)(UInt32() & 0xffff);
      }

      inline int32_t UInt16()
      {
        return (uint16_t)(UInt32() & 0xffff);
      }

      inline int32_t Int32()
      {
        return (int32_t)UInt32();
      }

      inline int32_t UInt32()
      {
        return NextRandom();
      }

      inline int64_t Int64()
      {
        return (int64_t)UInt64();
      }

      inline int32_t UInt64()
      {
        return ((uint64_t)UInt32() << 32) | (uint64_t)UInt32();
      }

      inline uint32_t NextRandom()
      {
        return Hash(index++, seed);
      }

    public:
      static Random globalRandom;

      static Random& Global() { return globalRandom; }

      // Based on Squirrel Eiserloh GDC talk
      static uint32_t Hash(int number, int seed)
      {
        number *= 0xb5297a4d;
        number += seed;
        number ^= number >> 8;
        number += 0x68e31da4;
        number ^= number << 8;
        number *= 0x1b56c4e9;
        number ^= number >> 8;
        return number;
      }
  };
}

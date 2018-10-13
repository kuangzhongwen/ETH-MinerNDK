/*
	This file is part of cpp-ethereum.

	cpp-ethereum is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	cpp-ethereum is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with cpp-ethereum.  If not, see <http://www.gnu.org/licenses/>.
*/
/** @file EthashAux.cpp
 * @author Gav Wood <i@gavwood.com>
 * @date 2014
 */

#include "EthashAux.h"
#include <libethash/internal.h>

using namespace std;
using namespace chrono;
using namespace dev;
using namespace eth;

EthashAux& EthashAux::get()
{
	static EthashAux instance;
	return instance;
}

h256 EthashAux::seedHash(unsigned _number)
{
	unsigned epoch = _number / ETHASH_EPOCH_LENGTH;
	EthashAux& ethash = EthashAux::get();
	Guard l(ethash.x_epochs);
	if (epoch >= ethash.m_seedHashes.size())
	{
		h256 ret;
		unsigned n = 0;
		if (!ethash.m_seedHashes.empty())
		{
			ret = ethash.m_seedHashes.back();
			n = ethash.m_seedHashes.size() - 1;
		}
		ethash.m_seedHashes.resize(epoch + 1);
		for (; n <= epoch; ++n, ret = sha3(ret))
			ethash.m_seedHashes[n] = ret;
	}
	return ethash.m_seedHashes[epoch];
}

uint64_t EthashAux::number(h256 const& _seedHash)
{
	EthashAux& ethash = EthashAux::get();
	Guard l(ethash.x_epochs);
	unsigned epoch = 0;
	auto epochIter = ethash.m_epochs.find(_seedHash);
	if (epochIter == ethash.m_epochs.end())
	{
		for (h256 h; h != _seedHash && epoch < 2048; ++epoch, h = sha3(h), ethash.m_epochs[h] = epoch) {}
		if (epoch == 2048)
		{
			std::ostringstream error;
			error << "apparent block number for " << _seedHash << " is too high; max is " << (ETHASH_EPOCH_LENGTH * 2048);
			throw std::invalid_argument(error.str());
		}
	}
	else
		epoch = epochIter->second;
	return epoch * ETHASH_EPOCH_LENGTH;
}

EthashAux::LightType EthashAux::light(h256 const& _seedHash)
{
	// TODO: Use epoch number instead of seed hash?

	EthashAux& ethash = EthashAux::get();
	Guard l(ethash.x_lights);
	if (ethash.m_lights.count(_seedHash))
		return ethash.m_lights.at(_seedHash);
	return (ethash.m_lights[_seedHash] = make_shared<LightAllocation>(_seedHash));
}

EthashAux::LightAllocation::LightAllocation(h256 const& _seedHash)
{
	uint64_t blockNumber = EthashAux::number(_seedHash);
	light = ethash_light_new(blockNumber);
	if (!light)
		BOOST_THROW_EXCEPTION(ExternalFunctionFailure("ethash_light_new()"));
	size = ethash_get_cachesize(blockNumber);
}

EthashAux::LightAllocation::~LightAllocation()
{
	ethash_light_delete(light);
}

bytesConstRef EthashAux::LightAllocation::data() const
{
	return bytesConstRef((byte const*)light->cache, size);
}

Result EthashAux::LightAllocation::compute(h256 const& _headerHash, uint64_t _nonce) const
{
	ethash_return_value r = ethash_light_compute(light, *(ethash_h256_t*)_headerHash.data(), _nonce);
	if (!r.success)
		BOOST_THROW_EXCEPTION(DAGCreationFailure());
	return Result{h256((uint8_t*)&r.result, h256::ConstructFromPointer), h256((uint8_t*)&r.mix_hash, h256::ConstructFromPointer)};
}

Result EthashAux::eval(h256 const& _seedHash, h256 const& _headerHash, uint64_t _nonce) noexcept
{
	try
	{
		return get().light(_seedHash)->compute(_headerHash, _nonce);
	}
	catch(...)
	{
		return Result{~h256(), h256()};
	}
}

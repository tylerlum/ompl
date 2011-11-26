/*********************************************************************
* Software License Agreement (BSD License)
*
*  Copyright (c) 2011, Willow Garage
*  All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*   * Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*   * Redistributions in binary form must reproduce the above
*     copyright notice, this list of conditions and the following
*     disclaimer in the documentation and/or other materials provided
*     with the distribution.
*   * Neither the name of the Willow Garage nor the names of its
*     contributors may be used to endorse or promote products derived
*     from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
*  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
*  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
*  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
*  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
*  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
*  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
*  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
*  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
*  POSSIBILITY OF SUCH DAMAGE.
*********************************************************************/

/* Author: Ioan Sucan */

#include "ompl/base/GenericParam.h"
#include <algorithm>

bool ompl::base::ParamSet::setParam(const std::string &key, const std::string &value)
{
    std::map<std::string, GenericParamPtr>::const_iterator it = params_.find(key);
    if (it != params_.end())
        return it->second->setValue(value);
    else
    {
        msg::Interface msg;
        msg.error("Parameter '" + key + "' was not found");
        return false;
    }
}

bool ompl::base::ParamSet::setParams(const std::map<std::string, std::string> &kv)
{
    bool result = true;
    for (std::map<std::string, std::string>::const_iterator it = kv.begin() ; it != kv.end() ; ++it)
    {
        bool r = setParam(it->first, it->second);
        result = result && r;
    }
    return result;
}

void ompl::base::ParamSet::getParamNames(std::vector<std::string> &params) const
{
    params.clear();
    params.reserve(params_.size());
    for (std::map<std::string, GenericParamPtr>::const_iterator it = params_.begin() ; it != params_.end() ; ++it)
        params.push_back(it->first);
    std::sort(params.begin(), params.end());
}

void ompl::base::ParamSet::getParamValues(std::vector<std::string> &vals) const
{
    std::vector<std::string> names;
    getParamNames(names);
    vals.resize(names.size());
    for (std::size_t i = 0 ; i < names.size() ; ++i)
        vals[i] = params_.find(names[i])->second->getValue();
}

const std::map<std::string, ompl::base::GenericParamPtr>& ompl::base::ParamSet::getParams(void) const
{
    return params_;
}

void ompl::base::ParamSet::getParams(std::map<std::string, std::string> &params) const
{
    for (std::map<std::string, GenericParamPtr>::const_iterator it = params_.begin() ; it != params_.end() ; ++it)
        params[it->first] = it->second->getValue();
}

void ompl::base::ParamSet::include(const ParamSet &other, const std::string &prefix)
{
    const std::map<std::string, GenericParamPtr> &p = other.getParams();
    if (prefix.empty())
        for (std::map<std::string, GenericParamPtr>::const_iterator it = p.begin() ; it != p.end() ; ++it)
            params_[it->first] = it->second;
    else
        for (std::map<std::string, GenericParamPtr>::const_iterator it = p.begin() ; it != p.end() ; ++it)
            params_[prefix + "." + it->first] = it->second;
}

void ompl::base::ParamSet::clear(void)
{
    params_.clear();
}

void ompl::base::ParamSet::print(std::ostream &out) const
{
    for (std::map<std::string, GenericParamPtr>::const_iterator it = params_.begin() ; it != params_.end() ; ++it)
        out << it->first << " = " << it->second->getValue() << std::endl;
}

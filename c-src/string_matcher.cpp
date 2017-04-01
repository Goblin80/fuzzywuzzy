#include "string_matcher.hpp"
#include "diffutils.hpp"

#include <algorithm>
#include <iostream>

namespace fuzz {

void string_matcher::set_strings(const string s1, const string s2)
{
    s1_ = s1;
    s2_ = s2;

    reset_cache();
}

void string_matcher::set_string1(const string s1)
{
    s1_ = s1;
    reset_cache();
}

void string_matcher::set_string2(const string s2)
{
    s2_ = s2;
    reset_cache();
}

void string_matcher::reset_cache()
{
    ratio_ = distance_ = 0;

    matching_blocks_.clear();
    op_codes_.clear();
    edit_ops_.clear();
}

vector<LevOpCode> string_matcher::get_opcodes()
{
    if (op_codes_.empty()) {
        if (!edit_ops_.empty())
            op_codes_ = diffutils::get_opcodes(edit_ops_, s1_, s2_);
        else
            op_codes_ = diffutils::get_opcodes(s1_, s2_);
    }

    return op_codes_;
}

vector<LevMatchingBlock> string_matcher::get_matching_blocks()
{
    if (matching_blocks_.empty())
        matching_blocks_ = diffutils::get_matching_blocks(get_opcodes(), s1_, s2_);
    return matching_blocks_;
}

double string_matcher::ratio()
{
    if (ratio_ == not_set)
        ratio_ = diffutils::ratio(s1_, s2_);
    return ratio_;
}

double string_matcher::real_quick_ratio()
{
    size_t len1 = s1_.length(), len2 = s2_.length();
    return 2.0 * std::min(len1, len2) / (len1 + len2);
}

}  // ns fuzz

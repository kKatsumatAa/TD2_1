#include "Input.h"

bool Input::ReleaseTriggerKey(BYTE keyNumber) const
{
    return (!key_[keyNumber] && keyPre_[keyNumber]);
}

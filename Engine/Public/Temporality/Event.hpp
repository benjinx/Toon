#ifndef EVENT_HPP
#define EVENT_HPP

//#include <Python.h>

#include <unordered_map>
#include <functional>
#include <type_traits>

class EventData {
public:
    template <class T>
    const T* GetAs() const {
        return dynamic_cast<const T*>(this);
    }

    //virtual PyObject* getPyObject() const = 0;
};

class ScriptEvent {
public:
    explicit ScriptEvent() = default;

    virtual ~ScriptEvent() = default;

    //unsigned AddPythonListener(PyObject* listener);

    //bool RemovePythonListener(PyObject* listener);

    bool RemovePythonListener(unsigned id);

    void CallPython(const EventData* data);

    void RemoveAllPythonListeners();

private:
    unsigned _mMaxPythonListenerID = 0;
    
    //std::unordered_map<unsigned, PyObject*> _mPythonListeners;
};

template <class EventDataType>
class Event : public ScriptEvent
{
    static_assert(
        std::is_base_of<EventData, EventDataType>::value,
        "EventDataType must be a descendant of EventData"
    );

public:
    explicit Event() = default;

    virtual ~Event() = default;

    typedef std::function<void(const EventDataType*)> Listener;

    unsigned AddListener(Listener listener) {
        ++_mMaxListenerID;
        _mListeners.emplace(_mMaxListenerID, listener);
        return _mMaxListenerID;
    }

    bool RemoveListener(unsigned id) {
        auto it = _mListeners.find(id);

        if (it != _mListeners.end()) {
            _mListeners.erase(it);
            return true;
        }

        return false;
    }

    void Call(const EventDataType* data) {
        for (const auto& it : _mListeners) {
            it.second(data);
        }

        CallPython(static_cast<const EventData*>(data));
    }

    void RemoveAllListeners() {
        _mListeners.clear();

        RemoveAllPythonListeners();
    }

private:
    unsigned _mMaxListenerID;

    std::unordered_map<int, Listener> _mListeners;
};

#endif // EVENT_HPP
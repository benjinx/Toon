#include <Toon/Event.hpp>

#include <Toon/Log.hpp>

//unsigned ScriptEvent::AddPythonListener(PyObject* listener)
//{
//    if (!PyCallable_Check(listener)) {
//        ToonLogError("PyObject not callable");
//        return 0;
//    }
//
//    ++_mMaxPythonListenerID;
//    Py_INCREF(listener);
//    _mPythonListeners.emplace(_mMaxPythonListenerID, listener);
//
//    return _mMaxPythonListenerID;
//}
//
//bool ScriptEvent::RemovePythonListener(PyObject* listener)
//{
//    auto it = _mPythonListeners.begin();
//    while (it != _mPythonListeners.end()) {
//        if (it->second == listener) {
//            break;
//        }
//    }
//
//    if (it != _mPythonListeners.end()) {
//        _mPythonListeners.erase(it);
//        return true;
//    }
//
//    return false;
//}

// bool ScriptEvent::RemovePythonListener(unsigned id)
// {
//     auto it = _mPythonListeners.find(id);

//     if (it != _mPythonListeners.end()) {
//        _nPythonListeners.erase(it);
//        return true;
//     }

//     return false;
// }

// void ScriptEvent::CallPython(const EventData * data)
// {
//     PyObject* dict = data->GetPyObject();
//     PyObject* arg = Py_BuildValue("(O)", dict);

//     ToonLogInfo("Calling Python Listeners");
//     for (const auto& it : _mPythonListeners) {
//        PyObject* result = PyEval_CallObject(it.second, arg);
//        if (!result) {
//            ToonLogError("Failed to call Python Event Listener");
//            PyPrintStackTrace();
//        }
//     }

//     Py_DECREF(arg);
//     Py_DECREF(dict);
// }

// void ScriptEvent::RemoveAllPythonListeners()
// {
//     for (const auto& it : _mPythonListeners) {
//        Py_DECREF(it.second);
//     }
//     _mPythonListeners.clear();
// }
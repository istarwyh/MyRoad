
### 1. Step
把ts服务器与客户端改造成java写的
凑文献


### 2. 未知的
#### 2.1. 读取pdf
elctron+pdf.js+云函数
React Native 做订阅
#### 2.2. 异步调用
```ts
function lazyHookCreation() {
  const inspector = internalBinding('inspector');
  const { createHook } = require('async_hooks');
  config = internalBinding('config');

  hook = createHook({
    init(asyncId, type, triggerAsyncId, resource) {
    // It's difficult to tell which tasks will be recurring and which won't,
    // therefore we mark all tasks as recurring. Based on the discussion
    // in https://github.com/nodejs/node/pull/13870#discussion_r124515293,
    // this should be fine as long as we call asyncTaskCanceled() too.
      const recurring = true;
      if (type === 'PROMISE')
        this.promiseIds.add(asyncId);
      else
        inspector.asyncTaskScheduled(type, asyncId, recurring);
    },

    before(asyncId) {
      if (this.promiseIds.has(asyncId))
        return;
      inspector.asyncTaskStarted(asyncId);
    },

    after(asyncId) {
      if (this.promiseIds.has(asyncId))
        return;
      inspector.asyncTaskFinished(asyncId);
    },

    destroy(asyncId) {
      if (this.promiseIds.has(asyncId))
        return this.promiseIds.delete(asyncId);
      inspector.asyncTaskCanceled(asyncId);
    },
  });

  hook.promiseIds = new Set();
}
```
为什么
```ts
 destroy(asyncId) {
      if (this.promiseIds.has(asyncId))
        return this.promiseIds.delete(asyncId);
      inspector.asyncTaskCanceled(asyncId);
    },
```
总是会调用下面的函数呢？
```typescript
// Called from native. The asyncId stack handling is taken care of there
// before this is called.
function emitHook(symbol, asyncId) {
  active_hooks.call_depth += 1;
  // Use a single try/catch for all hook to avoid setting up one per
  // iteration.
  try {
    for (var i = 0; i < active_hooks.array.length; i++) {
      if (typeof active_hooks.array[i][symbol] === 'function') {
        active_hooks.array[i][symbol](asyncId);
      }
    }
  } catch (e) {
    fatalError(e);
  } finally {
    active_hooks.call_depth -= 1;
  }

  // Hooks can only be restored if there have been no recursive hook calls.
  // Also the active hooks do not need to be restored if enable()/disable()
  // weren't called during hook execution, in which case
  // active_hooks.tmp_array will be null.
  if (active_hooks.call_depth === 0 && active_hooks.tmp_array !== null) {
    restoreActiveHooks();
  }
}
```
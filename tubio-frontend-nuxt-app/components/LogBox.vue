<template>
    <div class="log-box">
        <LogEntry mode="normal" message="Hello, lol :3" />
        <LogEntry mode="normal" message="Hello, lol :3" />
        <LogEntry mode="warn" message="Hello, lol :3" />
        <LogEntry mode="normal" message="Hello, lol :3" />
        <LogEntry mode="error" message="Hello, lol :3" />
        <LogEntry mode="normal" message="Hello, lol :3" />
        <LogEntry mode="normal" message="Hello, lol :3" />
        <LogEntry mode="normal" message="Hello, lol :3" />
    </div>
</template>

<script>
import LogEntry from "~/components/LogEntry";

export default {
    components: {
      LogEntry,
  }
  
}
</script>

<style lang="scss" scoped>
.log-box {
  width: 100%;
  height: 600px;
  border-radius: 5px;
  padding: 20px;
  background-color: #fff5;
  overflow-y: scroll;
}
</style>
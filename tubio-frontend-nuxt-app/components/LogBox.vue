<template>
    <div class="log-box">
        <LogEntry v-for="(nObj, nKey) in logs" 
          :mode="(nObj.type === 0) ? 'normal' : ((nObj.type === 1) ? 'warn' : 'error')"
          :message="nObj.compiledMessage"
          :key="nKey" />
    </div>
</template>

<script>
import LogEntry from "~/components/LogEntry";

export default {
    components: {
      LogEntry,
  },

  computed: {
    logs: function() {
      return this.$store.state.logs.logs;
    }
  },

  mounted() {
    const that = this;
    this.$store.dispatch("logs/update");
    setInterval(function(){
      that.$store.dispatch("logs/update");
    }, 1000);
    return;
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
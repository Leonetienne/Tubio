<template>
    <div class="download-box">
        <h2 v-if="false" class="no-dls-yet mt-2">No downloads yet...</h2>

        <DownloadEntry />
        <DownloadEntry />
        <DownloadEntry />
        <DownloadEntry />
        <DownloadEntry />
        <DownloadEntry />
        <DownloadEntry />
        <DownloadEntry />
        <DownloadEntry />
        <DownloadEntry />
        <DownloadEntry />
        <DownloadEntry />
        <DownloadEntry />
        <DownloadEntry />
        <DownloadEntry />
        <DownloadEntry />
        <DownloadEntry />
    </div>
</template>

<script>
import DownloadEntry from "~/components/DownloadEntry";

export default {
    components: {
      DownloadEntry,
  }
  
}
</script>

<style lang="scss" scoped>
.download-box {
  width: 100%;
  min-height: 600px;
  border-radius: 5px;
  padding: 20px;
  background-color: #fff5;
}

.no-dls-yet {
  color: theme("colors.text-gray-1");
  font-size: 34pt;
  text-align: center;
}
</style>
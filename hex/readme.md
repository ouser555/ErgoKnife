## 測試版
  * 前一版反映有副手鍵盤Trackpoint漂移的情況，但手邊沒有這款鍵盤可以測試，就只能提供測試黨讓使用者自己測試看看能否解決著個問題。
    * 前一版的修改為
      * /rev1/config.h
        ```
        //#define SPLIT_USB_DETECT
        //#define USB_SUSPEND_WAKEUP_DELAY 1000
        //#define SPLIT_USB_TIMEOUT 2000
        //#define SPLIT_USB_TIMEOUT_POLL 10
        //#define SPLIT_WATCHDOG_ENABLE
        //#define SPLIT_WATCHDOG_TIMEOUT 3000
        ```
      * /rule.mk
        ```
        WAIT_FOR_USB = yes
        ```
    * 測試版就是把這幾行拿掉
      https://github.com/ouser555/ErgoKnife/blob/main/hex/ergoknife_rev1_v99-20230411-test.hex
      
  
## 更新通知 20230123
  #### 配合使用VIA configurator 新版本2.10，新的via json設定檔
  
  #### 使用當前最新版本的QMK，它已經將副手的Trackpoint裝置會lag的問題改善了
  
  #### 限制RGB最大電流，燈光調到最大不會因過載而斷線
  
  * 燒錄檔下載位置
    https://github.com/ouser555/ErgoKnife/blob/main/hex/ergoknife_rev1_v99-20230123.hex
    
    (對畫面右上方的RAW按鈕，右鍵另存連結為...)
  
  * 下載VIA 2.10版
    https://github.com/the-via/releases/releases/
    
  * 下載新的json檔
    https://github.com/ouser555/ErgoKnife/blob/main/via%20config/ErgoKnife-20230303.json
    
    (對畫面右上方的RAW按鈕，右鍵另存連結為...)
    
  * 也可以使用網頁版的VIA
    https://usevia.app/#/
  
## 更新通知 20221101
  #### 修復左右手鍵盤通訊延遲的問題

  #### 更新OLED顯示介面，能更清楚的顯示目前鍵盤在何種模式。

  #### 燒錄檔下載位置
  https://github.com/ouser555/ErgoKnife/blob/main/hex/ergoknife_rev1_v99-20221101.hex

    在網頁介面右方有一個Raw的按鈕，滑鼠按右鍵另存新檔即可下載燒錄檔，
    (不要在這個網頁使用其他操作方式下載，可能會下載到錯誤的檔案)

  #### 重新燒錄任體會將你原本改過的鍵盤設定清除掉，更新你的鍵盤之前，
  ~~建議可以先用VIA configuration儲存你使用中的keymap，待更新之後再複寫回來~~。
  
  20230303 因為最近via從v2更新到v3，你的鍵盤有些功能可能要自己重新設定  

  #### 燒錄教學
  https://github.com/ouser555/ergodashv99/blob/main/QMKToolbox/readme.md
